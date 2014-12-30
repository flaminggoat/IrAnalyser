//Theo Hussey 2014
#include <SPI.h>
#include <TFT_ILI9163C.h>
#include <Adafruit_GFX.h>
#include "IrAnalyser.h"

#define __CS 5
#define __DC 3
#define __RST 4

// Color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF

#define TO_MICRO(counter) (counter*64)/3

TFT_ILI9163C display = TFT_ILI9163C(__CS, __DC, __RST);

uint8_t necByteDecode(uint8_t * byte) {
	uint16_t time;
	*byte = 0;
	for(int i=8; i>0; i--) {
		*byte >>= 1;
		
		while(!(PIND & _BV(TSOP38238)));
		time = TO_MICRO(TCNT1);
		TCNT1 = 0; //Reset timer 1
		if(time > 670 || time < 470)
			return 4; //Invalid 570us burst

		while(PIND & _BV(TSOP38238));
		time = TO_MICRO(TCNT1);
		TCNT1 = 0; //Reset timer 1
		if(time < 470)
			return 5; //Invalid 570us delay
		if(time < 700); //valid "0"
		else if(time < 1790)
			*byte |= 0x80; //valid "1"
		else
			return 5; //Invalid 1690us delay
	}
	return 0;
}

uint8_t necDecode(necPacket * packet) {
	TCNT1 = 0;
	uint16_t time;
	while(!(PIND & _BV(TSOP38238)));
	time = TO_MICRO(TCNT1);//Convert into micro seconds
	TCNT1 = 0;
	if(time > 9300 || time < 8700)
		return 2; //Invalid 9ms burst
    
	while(PIND & _BV(TSOP38238));
	time = TO_MICRO(TCNT1);
	TCNT1 = 0;
	if(time < 2000)
		return 3;
	if(time < 3000) {
		while(!(PIND & _BV(TSOP38238)));
		return 1;//Repeat signal
	}
	if(time > 4700 || time < 4300)
		return 3; //Invalid 4.5ms delay
		
	uint8_t byte1, byte2, error;
	
	if(error = necByteDecode(&byte1)) //Decode 1st id byte
		return error;	
	if(error = necByteDecode(&byte2)) //Decode 2nd id byte which is the inverse of the first
		return error;
		
	if((byte1 ^ byte2) != 0xff) {
		//Assume NEC extended and that Id verification hasn't failed
		packet->device = byte1;
		packet->device |= ((uint16_t) ~byte2) << 8;
	} else {
		//Standard NEC with 8bit id
		packet->device = byte1;
	}
	
		
	if(error = necByteDecode(&byte1)) //Decode 1st data byte
		return error;	
	if(error = necByteDecode(&byte2)) //Decode 2nd data byte which is the inverse of the first
		return error;
		
	if((byte1 ^ byte2) != 0xff)
		return 6; //Data verification failed
	packet->data = byte1;

	return 0;
}

void setup() {
  //IO
  //pinMode(RED_LED, OUTPUT);
  pinMode(TSOP38238, INPUT);
  Serial.begin(9600);
  
  display.begin();
  display.setRotation(2); //Dosent seem to work properly the other way up
  display.clearScreen();
  display.setCursor(0,0);
  display.setTextColor(WHITE);  
  display.setTextSize(1);
  display.println("IR Analyser");
  
  //Timer 1 setup
  // disable global interrupts
  cli();
  //Output compare off, PWM off
  TCCR1A = 0;
  //divide clock by 256
  TCCR1B = 0x04;
  // no interrupts
  TIMSK1 = 0;
  // enable global interrupts:
  sei();
}


void loop() {
	static uint8_t line = 1;
	static char buffer[40];
	
	if(!digitalRead(TSOP38238))
	{	
		necPacket p;
		uint8_t err;
		err = necDecode(&p);
		if(err > 1) {
			Serial.print("Error = ");
			Serial.println(err); 
		} else if(err == 1) {
			Serial.println("Repeat"); 
		} else {
			sprintf(buffer, "Device = 0x%x\n\rData   = 0x%x\n\r", p.device, p.data);
			line += 2;
			
			//display.setTextColor(YELLOW);
			//display.print("Device = 0x");
			//display.println(p.device, HEX);
			//display.setTextColor(GREEN);
			//display.print("Data   = 0x");
			//display.println(p.data, HEX);
			//line += 2;
			
			//Serial.print("Device = 0x");
			//Serial.println(p.device, HEX);
			//Serial.print("Data   = 0x");
			//Serial.println(p.data, HEX);
		}
		//delay(100);
	}
	
	if(*buffer) {
		if(line > 16) {
			line = 0;
			display.clearScreen();
			display.setCursor(0,0);
		}
		display.print(buffer);
		Serial.print(buffer);
		*buffer = 0;
	}
	
	
}
