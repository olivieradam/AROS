/*
    Copyright (C) 2013, The AROS Development Team. All rights reserved.
*/

#ifndef SERIALDEBUG_H_
#define SERIALDEBUG_H_

#include <inttypes.h>

extern void serInit();
//void waitBusy();
extern void waitSerIN();
extern void waitSerOUT();
extern void putByte(uint8_t chr);

#endif /* SERIALDEBUG_H_ */
