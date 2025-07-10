//Yusuf Semo 1001993176 sending IR singal
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "clock.h"
#include "uart0.h"
#include "command_term.h"
#include "IRsend.h"

int main(void)
{
    initSystemClockTo40Mhz();
    initIRTransmitter();
    //send netflix code to turn on the TV and put netflix and chill
    uint8_t message[] = { 0x20 };
    uint8_t message2[] = { 0xDF };
    uint8_t message3[] = { 0x6A };
    uint8_t message4[] = { 0x95 };

    while (1)
    {
        sendNECPreamble();
        sendIRCommand(message, 1);
        sendIRCommand(message2, 1);
        sendIRCommand(message3, 1);
        sendIRCommand(message4, 1);
        sendEndBit();
        //delay
        delayMicroseconds(100000);
    }
}
