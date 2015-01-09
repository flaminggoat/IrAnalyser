//IrAnalyser
//Theo Hussey 2014
#include "IrAnalyser.h"

#include <SPI.h>
#include <TFT_ILI9163C.h>
#include <Adafruit_GFX.h>
#include <IRremote.h>

TFT_ILI9163C display = TFT_ILI9163C(__CS, __DC, __RST);
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  
  display.setBitrate(8000000);
  display.begin();
  display.setRotation(2); //Doesn't seem to work properly the other way up
  display.clearScreen();
  display.setCursor(0,0);
  display.setTextColor(RED);  
  display.setTextSize(1);
  display.println("IR Analyser");

  irrecv.enableIRIn(); // Start the receiver
}

void printProtocol(int8_t protocol) {
	switch(results.decode_type) {
		case NEC:
			display.setTextColor(BLUE);
			display.println("NEC");
			break;
		case SONY:
			display.setTextColor(GREEN);
			display.println("SONY");
			break;
		case RC5:
			display.setTextColor(CYAN);
			display.println("RC5");
			break;
		case RC6:
			display.setTextColor(MAGENTA);
			display.println("RC6");
			break;
		case DISH:
			display.setTextColor(YELLOW);
			display.println("DISH");
			break;
		case SHARP:
			display.setTextColor(BLUE);
			display.println("SHARP");
			break;
		case PANASONIC:
			display.setTextColor(GREEN);
			display.println("PANASONIC");
			break;
		case JVC:
			display.setTextColor(CYAN);
			display.println("JVC");
			break;
		case SANYO:
			display.setTextColor(MAGENTA);
			display.println("SANYO");
			break;
		case MITSUBISHI:
			display.setTextColor(YELLOW);
			display.println("MITSUBISHI");
			break;
		case SAMSUNG:
			display.setTextColor(BLUE);
			display.println("SAMSUNG");
			break;
		case LG:
			display.setTextColor(GREEN);
			display.println("LG");
			break;
		default:
			display.setTextColor(RED);
			display.println("UNKNOWN");
			break;
	}

	display.setTextColor(WHITE);
}

void loop() {
	static int8_t debug = 0;
	static int8_t protocol = -1;

	static int8_t line = 0;

	if (irrecv.decode(&results)) {
		if(line > 15) {
			//Clear screen if cursor has gone off the bottom
			display.clearScreen();
			display.setCursor(0,0);
			printProtocol(protocol);
			line = 1;
		}

		if(protocol != results.decode_type) {
			//Protocol has changed since the last signal
			protocol = results.decode_type;
			printProtocol(protocol);
			line++;
		}

		if(protocol != UNKNOWN) {
			display.println(results.value, HEX);
			line++;
			Serial.println(results.value, HEX);
	    } else {
	    	//TODO: Display unknown protocols in a more informative manner
	    	for(int i=0; i<results.rawlen; i++)
	    		display.print(results.rawbuf[i],HEX);
	    	display.print("\n");
	    	line++;
	    }

	    irrecv.resume(); // Receive the next value
	}
}
