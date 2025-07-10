// systick.c
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "systick.h"
// put this for red green and blue in your main program so it knows what led to turn on
#define GREEN_LED     (*((volatile uint32_t *)(0x42000000 + (0x253FC)*32 + 3*4)))

#define RED_LED_MASK    0x02
#define BLUE_LED_MASK   0x04
#define GREEN_LED_MASK  0x08
volatile uint32_t msTicks = 0;  //millisecond counter


// we are going to init this systick so it creates a 38kHz signal
void init_systick38k()
{
    NVIC_ST_CTRL_R = 0;                         // Disable SysTick
    NVIC_ST_RELOAD_R = 0xFFFFFF;                // Set max reload (just a placeholder)
    NVIC_ST_CURRENT_R = 0;                      // Clear current value
    NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE; // Enable with system clock, no interrupt
}

////delay time from the systick
//void delayMicroseconds(uint32_t us)
//{
//    NVIC_ST_RELOAD_R = (40 * us) - 1;   // 40 cycles per 1 microsecond
//    NVIC_ST_CURRENT_R = 0;              // Clear current value
//    NVIC_ST_CTRL_R = 0x05;              // Enable SysTick: Core clock, no interrupt
//
//    while ((NVIC_ST_CTRL_R & 0x10000) == 0);  // Wait for COUNT flag
//}

void initled(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    _delay_cycles(3);
    GPIO_PORTF_DIR_R |= (BLUE_LED_MASK | RED_LED_MASK | GREEN_LED_MASK);
    GPIO_PORTF_DEN_R |= (BLUE_LED_MASK | RED_LED_MASK | GREEN_LED_MASK);
}

void SysTick_Handler(void)
{
    msTicks++;
    //if 1 second passes turn the green led on or off
    if(msTicks > 1000)
    {
        //reset timer
        msTicks = 0;
        if(GREEN_LED)
        {
            GREEN_LED = 0;
        }
        else
        {
            GREEN_LED = 1;
        }
    }


}

uint32_t currentTime(void)
{
    msTicks = msTicks;
    return msTicks;
}

uint32_t deltaTime(uint32_t t0, uint32_t t1)
{
    return t1 - t0;
}

