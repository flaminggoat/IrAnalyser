//IrAnalyser
//Theo Hussey 2014
#include "IrAnalyser.h"

#include <SPI.h>
#include <TFT_ILI9163C.h>
#include <Adafruit_GFX.h>

#include "NecDecode.h"

TFT_ILI9163C display = TFT_ILI9163C(__CS, __DC, __RST);

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
	static const uint8_t debug =0;
	static uint8_t line = 1;
	static char buffer[40];
	static uint16_t offset = 0;
	
	if(!digitalRead(TSOP38238))
	{	
		necPacket p;
		uint8_t err;
		err = necDecode(&p);
		if(err > 1) {
			if(debug) {
				offset += sprintf(buffer + offset, "Error = %d\n", err);
				line ++;
			}
			//Serial.print("Error = ");
			//Serial.println(err); 
		} else if(err == 1) {
			offset += sprintf(buffer + offset, "Repeat\n");
			line ++;
			//Serial.println("Repeat"); 
		} else {
			offset += sprintf(buffer + offset, "Device = 0x%x\n\rData   = 0x%x\n\r", p.device, p.data);
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
		offset = 0;
	}
	
	
}
