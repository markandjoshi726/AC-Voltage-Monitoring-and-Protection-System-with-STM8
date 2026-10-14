# AC voltage monitoring, STM8 Embedded C example

Included files: `main.c` and `config.h`.

This example follows the user's major project report (Chapter 3.2 and Chapter 4.2). It implements ADC polling, voltage scaling, three-digit display multiplexing, and these two-relay rules:

| Estimated AC voltage | Relay 1 | Relay 2 |
|---|---|---|
| Below 240 V | OFF | ON |
| 240 through 245 V | ON | OFF |
| Above 245 V | OFF | OFF |

## Build in IAR

Use **IAR Embedded Workbench for STM8**, not IAR for ARM. Create a new project with the precise target device and add `main.c` to the project. Keep `config.h` in the same directory. IAR generates its own `.ewp` and `.eww` project/workspace files.

## Assumptions needing verification

- The example assumes **STM8S003F3P6**, which the report does not conclusively identify.
- Pin assignments are **examples**, not verified from the source schematic. ADC PD2/AIN3, relays PA1/PA2, segment A PA3, B/C PB4/PB5, D/E/F/G/DP PC3-PC7, digit selects PD4/PD5/PD6.
- The display is assumed common-anode, active-low segments, active-high digit selection. Relay drive is assumed active high.
- ADC reference is assumed 5.0 V and must be measured/calibrated.
- The report gives the voltage divider ratio as 0.0105, and the example approximates RMS using peak / sqrt(2). This is accurate only when the sensed rectified DC tracks the AC peak, waveform is sinusoidal and filtering/calibration are appropriate. Average samples alone do not compute true RMS.
- No hysteresis, timed display ISR, brownout recovery policy, watchdog servicing, independent protection path, sensor open-circuit detection, or verified mains fail-safe handling is included. **Do not deploy as a protection product.**
- This example has not been compiled/tested in IAR or validated on physical hardware.

## Critical safety

The report describes a non-isolated NCP1060 supply: the STM8 side may be **mains live**. Do not attach an earthed oscilloscope, SWIM/USB debugger or PC while powered from mains. Program with mains power disconnected; have qualified personnel set up any high-voltage tests with appropriate equipment.
