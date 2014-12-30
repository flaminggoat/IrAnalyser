//NecDecode
//Theo Hussey 2014

#ifndef NEC_DECODE_H
#define NEC_DECODE_H

#include <stdint.h>

#define TSOP38238 6

#define TO_MICRO(counter) (counter*64)/3

typedef struct{
  uint16_t device;
  uint8_t data;
}necPacket;

uint8_t necDecode(necPacket * packet);

#endif