//NecDecode
//
//
//
//Theo Hussey 2014
/*
#include "NecDecode.h"

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
}*/