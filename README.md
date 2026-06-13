# Digital Event Notice Board

A modular, bare-metal firmware system engineered for the NXP ARM7 LPC2148 microcontroller. The system pairs real-time clock tracking with an asynchronous, non-blocking text-scrolling engine to manage time-critical schedule notifications on a physical 16x2 HD44780 LCD workspace.

Rather than relying on classic blocking delay routines that stall processor execution, this firmware uses distinct vectored interrupts and state-machine scheduling to keep background data sampling fluid while scrolling text strings smoothly.

---

## Hardware Profile & Peripherals Mapping

* **Microcontroller:** NXP ARM7 LPC2148 (Core frequency geared to 60 MHz via PLL0; Peripheral Clock vPB running at 15 MHz).
* **Display Layout:** 16x2 HD44780 LCD running on an 8-bit parallel data bus for ultra-low latency panel updates.
* **User Input:** 4x4 Matrix Keypad mapped to dedicated GPIO rows, using software debouncing to manage physical button spring noise.
* **Timing & Schedulers:** On-chip Hardware Real-Time Clock (RTC) for standalone calendar tracking alongside Timer0 Match Registers for countdown calculations.
* **Sensor Diagnostics:** 10-bit Successive Approximation On-chip Analog-to-Digital Converter (ADC0, Channel 2).

---

## Core Firmware Engines

### 1. Text Scrolling Algorithm

Displaying strings longer than the 16-character hardware boundary requires a non-destructive memory-sliding window. The system handles this operation by translating character strings onto the display grid step-by-step and shifting the base string memory pointer forward across the buffer array to create a continuous scrolling effect.

### 2. Asynchronous Interrupt Design (__irq)

To maximize efficiency, blocking loops have been stripped out of the main routine and mapped directly to independent hardware interrupt service routines (ISRs) inside the Vectored Interrupt Controller (VIC):

* **`rtc_alarm_isr` (Slot 1):** Monitors scheduled schedules in the background every minute. When a match occurs, it asserts the `startScroll` flag and breaks immediately to allow text parsing.
* **`eint0_isr` (Slot 0):** Mapped to an edge-sensitive, active-low external push-button. Pressing the key suspends standard time loops immediately and shifts execution into the interactive configuration menu.
* **`Timer0_ISR` (Slot 2):** Configured with a Prescale Value of 14,999 to divide the 15 MHz peripheral clock into a precise 1 kHz (1ms) time base. The Match Register hits at 1000 counts to generate an exact 1-second pulse for countdown tracking.

### 3. Non-Blocking Sensor Interleaving

Rather than stalling execution using a hard delay for sensor updates, the background core utilizes an automated time-slice verification filter (`sec % 2 == 0`). The ADC reads the analog input channel only during even seconds, keeping temperature/sensor diagnostics running seamlessly without causing stutter on the scrolling display.

---

## Repository Structure

The repository maintains all necessary project and source files directly in a flat directory layout for deployment:

* **Source Files (`.c`)** -> Implementation code including `eb_main.c`, `lcd.c`, `kpm.c`, `adc.c`, `lcd_scrolling.c`, `rtc_lcd.c`, `admin_mode.c`, `rtc_interrupt.c`, `ext_int0.c`, `timer_interrupt.c`, and `countdown.c`.
* **Header Files (`.h`)** -> Interface definitions and register mappings including `types.h`, `delay.h`, `global.h`, `msg.h`, `defines.h`, `lcd_defines.h`, `kpm_defines.h`, and `adc_defines.h`.
* **`Startup.s`** -> Target-specific ARM assembly language file that configures stack pointers and handles vector table mapping before jumping to `main()`.
* **`event_board_final.hex`** -> The precompiled execution binary loaded directly onto the target hardware framework.

---

## Hardware Validation & Production States

* **Standby Mode:** Displays real-time current clock metrics, full calendar date matrices, and ongoing ambient sensor readouts. A Red status LED remains lit to show the system is waiting for an alarm event.
* **Active Notice Mode:** When a clock time matches a scheduled event, the system shifts states. A Green status LED fires up, the active notice message rolls smoothly on Line 1, and an active countdown timer displays on Line 2.
* **Admin Configuration Dashboard:** Triggered via the `EINT0` button, allowing users to safely type in new clock data sets, change calendar configurations, or enable/disable specific message blocks on the fly using the matrix keypad.
