//functions to send an IR signal
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "clock.h"

// 1. Delay function using SysTick (for 40 MHz system clock)
void delayMicroseconds(uint32_t us)
{
    NVIC_ST_RELOAD_R = (40 * us) - 1;    // 40 MHz = 25 ns per tick
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 0x05;               // Enable, no interrupt, use system clock
    while ((NVIC_ST_CTRL_R & 0x10000) == 0);  // Wait for COUNT flag
}

// 2. Initialize PC4 for IR LED output the gpio pin
void initIRTransmitter()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;   // Enable clock for Port C
    _delay_cycles(3);
    GPIO_PORTC_DIR_R |= (1 << 4);              // Set PC4 as output
    GPIO_PORTC_DEN_R |= (1 << 4);              // Enable digital function on PC4
    GPIO_PORTC_DATA_R &= ~(1 << 4);            // Start with PC4 LOW
}



// 3. Send 38 kHz carrier for a specific duration
void send38kBurst(uint32_t duration_us)
{
    uint32_t cycles = duration_us / 28;
    uint32_t i = 0;
    for (i = 0; i < cycles; i++)
    {
        GPIO_PORTC_DATA_R |= (1 << 4);   // Turn ON IR LED
        delayMicroseconds(13);           // Half-period delay
        GPIO_PORTC_DATA_R &= ~(1 << 4);  // Turn OFF IR LED
        delayMicroseconds(13);           // Half-period delay
    }
}


void sendNECPreamble()
{
    send38kBurst(9000);            // 9 ms carrier
    GPIO_PORTC_DATA_R &= ~(1 << 4);
    delayMicroseconds(4500);       // 4.5 ms gap
}

// Send IR data using modulation
void sendIRCommand(uint8_t* data, uint8_t length)
{
    uint8_t i = 0;
    for (i = 0; i < length; i++)
    {
        uint8_t byte = data[i];
        int8_t bit = 7;
        for (bit = 7; bit >= 0; bit--)
        {
            // Always send 560us of 38kHz
            send38kBurst(560);

            // Turn off GPIO (LOW) for duration depending on bit value
            GPIO_PORTC_DATA_R &= ~(1 << 4);

            if ((byte >> bit) & 1)
                delayMicroseconds(1690);  // Logic 1
            else
                delayMicroseconds(560);   // Logic 0
        }
    }
}

void sendEndBit()
{
    send38kBurst(560);                // Final 560 us carrier burst
    GPIO_PORTC_DATA_R &= ~(1 << 4);   // Keep IR LED off afterward
}
// receave code couldn't get working ISR wouldn't interupt

void initReceiveTimer()
{
    // Disable the timer before config
    WTIMER2_CTL_R &= ~TIMER_CTL_TBEN;

    // Configure as 16/32-bit timer
    WTIMER2_CFG_R = 0x4;

    WTIMER2_TBMR_R = TIMER_TBMR_TBMR_CAP     // 0x3 = edge-time mode
                   | TIMER_TBMR_TBCMR;        // 0x4 = capture mode

    WTIMER2_CTL_R &= ~TIMER_CTL_TBEVENT_M;    // Clear event settings
    WTIMER2_CTL_R |= TIMER_CTL_TBEVENT_POS;   // Set to capture rising edges only

    WTIMER2_TBPR_R = 0;

    WTIMER2_TBILR_R = 0xFFFFFFFF;

    WTIMER2_IMR_R |= TIMER_IMR_CBEIM;
    NVIC_EN3_R |= (1 << (INT_WTIMER2B - 96));

    WTIMER2_CTL_R |= TIMER_CTL_TBEN;
}


void initPD1forCapture()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;   // Enable clock for Port D
    _delay_cycles(3);

    GPIO_PORTD_AFSEL_R |= (1 << 1);            // Enable alt function
    GPIO_PORTD_PCTL_R &= ~(0xF << 4);          // Clear PCTL for PD1
    GPIO_PORTD_PCTL_R |= (0x7 << 4);           // Assign WT2CCP1 function (ALT 7)
    GPIO_PORTD_DEN_R |= (1 << 1);              // Digital enable
    GPIO_PORTD_DIR_R &= ~(1 << 1);             // Set as input
    GPIO_PORTD_AMSEL_R &= ~(1 << 1);            // **Disable analog mode**
}


