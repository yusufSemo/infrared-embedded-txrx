# Infrared Signal Decoder & Transmitter for Embedded Systems

This project implements an IR signal transmitter and decoder using the **TM4C123GXL (Tiva-C)** microcontroller. It sends NEC protocol IR signals using a hand-soldered IR LED circuit and receives IR codes using a TSOP134 receiver module. Useful for controlling TVs or learning remote codes.

![IR Transmitter Circuit](circuit_diagram_tx.jpg)  
![IR Receiver Circuit](circuit_diagram_rx.jpg)

> Developed by **Yusuf Semo** — built entirely from scratch using low-level C, GPIO, PWM, and UART. Designed for embedded learning, IR remote control emulation, and custom hardware development.

---

## 🧠 Features

- Sends **NEC IR protocol** with 4 custom hex values
- Uses **UART** terminal interface (115200 baud) to type commands like:  
  `send 20 DF 6A 95`
- Transmits each command **10 times** to increase reliability
- Receives IR codes using **input capture ISR** with microsecond resolution
- Fully built in **Code Composer Studio** for the TM4C123GXL
- Hand-soldered IR circuits on perfboard
- Modular, low-level embedded C (no `scanf`, no standard C library)

---

## 🧰 Hardware Used

| Component             | Description                                     |
|-----------------------|-------------------------------------------------|
| **TM4C123GXL**        | Tiva-C LaunchPad (main MCU)                     |
| **IR333-A**           | High-intensity 940nm IR LED (transmitter)       |
| **TSOP13438**         | 38kHz modulated IR receiver module              |
| Custom Circuit        | Hand-soldered components with perfboard         |
| UART Serial Terminal  | Used for interactive commands & debugging       |

📄 Included Docs:
- [IR333-A Datasheet](IR333_A_datasheet.pdf)
- [TSOP134 Receiver Datasheet](tsop134.pdf)
- [TI NEC IR Protocol App Note](slaa644b.pdf)

---

## 💡 How It Works

### Transmitter (IR LED)
- Generates **38kHz carrier** using GPIO toggling
- NEC protocol: 9ms pulse + 4.5ms space → 4 data bytes + stop bit
- Signal structure:
[9ms] [4.5ms] [bit 1] [bit 2] ... [bit 32] [stop]

Each command is repeated **10x** in a row to ensure the receiving TV or device catches at least one clean transmission.

### Receiver (IR Decode)
- TSOP134 output is connected to a GPIO input pin with **edge-time capture**
- **Wide Timer 2B ISR** captures timestamps of rising edges
- Compares timing gaps to detect logical '1' and '0' values
- Decodes into full 32-bit IR command

---

## 🧪 Usage

### 1. Open in Code Composer Studio
- Clone or download the project
- Import as an **existing CCS project**
- Connect your TM4C123 LaunchPad

### 2. Flash the firmware

### 3. Open UART terminal (Tera Term / PuTTY)
- **Baud Rate**: 115200  
- Type this to send signal:
send 20 DF 6A 95

### 4. Aim the IR LED at your device (TV, etc.)

---

## 🔧 Wiring Overview

### IR Transmitter (TX)
- **IR333 LED** → PC4 (GPIO)
- Resistor (100–220Ω)
- No external driver needed (driven directly by GPIO)

### IR Receiver (RX)
- **TSOP134 OUT** → PD1 (Wide Timer 2B CCP input)
- Powered by 5V
- Pull-down or series resistor to reduce spiking

---

## 📸 Project Images

| ![Transmitter](img/tx_board.jpg) | ![Receiver](img/rx_module.jpg) |
|----------------------------------|--------------------------------|

---

## 🚀 Possible Extensions

- Support for other IR protocols (Sony, RC-5, etc.)
- Decode and retransmit arbitrary signals
- Store captured signals in EEPROM or Flash
- Display learned codes on OLED screen
- Wi-Fi controlled IR blaster for smart home use

---

## 🛠️ Tech Stack

- **Code Composer Studio (TI)**  
- **Bare-metal C(No Libraries)**  
- **TM4C123 Tiva C LaunchPad**  
- UART, GPIO, SysTick, Wide Timers, PWM (manual), Interrupts
