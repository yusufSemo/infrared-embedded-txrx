#ifndef IRSEND_H
#define IRSEND_H

#include <stdint.h>
#include <stdbool.h>

// Initializes PC4 as output to drive the IR LED circuit
void initIRTransmitter(void);

// Delays for a specified number of microseconds (uses SysTick)
void delayMicroseconds(uint32_t us);

// Sends a 38 kHz square wave for the given duration (in microseconds)
void send38kHzBurst(uint32_t duration_us);

// Sends a single bit as a burst + gap (0 or 1)
void sendBit(bool bit);

void sendNECPreamble();

// Sends an array of bytes as an IR message (MSB first)
void sendIRCommand(uint8_t* data, uint8_t len);
void sendEndBit();
void initPD1forCapture();

#endif
