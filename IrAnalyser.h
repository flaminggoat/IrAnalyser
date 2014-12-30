#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED 13
#define TSOP38238 6

typedef struct{
  uint16_t device;
  uint8_t data;
}necPacket;

#endif;
