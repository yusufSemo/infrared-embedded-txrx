// systick.h
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

//initializes SysTick to trigger every 1 ms
void init_systick38k(void);
void initled(void);
void SysTickHandler(void);

// Returns the current system time in milliseconds
uint32_t currentTime(void);

//returns the number of milliseconds between two times
uint32_t deltaTime(uint32_t t0, uint32_t t1);


#endif
