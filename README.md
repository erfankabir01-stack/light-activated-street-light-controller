
# Light-Activated Street Light Controller


LM358 + LDR based street light controller with ATmega328P fault detection, simulated in Proteus 8.
A light-sensing street light controller built and simulated in **Proteus 8**, using an **LM358 op-amp** as a comparator with an **LDR (Light Dependent Resistor)** as the sensing element. Extended with an **ATmega328P**-based fault detection subsystem that monitors the whole circuit — lamp, indicator LEDs, and the sensor itself — for faults in real time.

## What it does

- The LDR senses ambient light. The LM358 op-amp compares its voltage against a reference threshold.
- Below the threshold (dusk), the street light turns **ON**. Above it (dawn), the light turns **OFF**.
- An ATmega328P microcontroller runs alongside the analog circuit, continuously checking for three kinds of faults:
  - **Lamp fault** — supply voltage is present but no current is actually flowing (burnt-out / open lamp)
  - **LED fault** — same current-vs-command check applied to the two status indicator LEDs (D2, D3)
  - **Sensor fault** — the LDR's voltage sits at a value a real LDR could never physically produce (shorted or disconnected)
- Any fault drives a dedicated output pin HIGH, and live diagnostics are printed to the Virtual Terminal at 9600 baud for easy debugging in Proteus.

## Tools used

- **Proteus 8** — circuit design and simulation
- **LM358** — dual op-amp used as a voltage comparator
- **LDR** — light sensing
- **ATmega328P** (Arduino) — fault detection and serial diagnostics

## Contents of this repo

- `EEE234 Project Sim.pdsprj` — Proteus simulation file
- `EEE234_Project_Report.docx` — full IEEE-format project report
- `Screenshot 2026-07-14 164448.png`, `Screenshot 2026-07-15 212953.png` — circuit screenshots from simulation
- `fault_monitor.ino` — ATmega328P fault detection code (lamp, LED, and sensor monitoring)

## Background

This project was completed as part of coursework (EEE234 – Electronics) at Independent University, Bangladesh (IUB).

## Author

Mir Erfan Kabir Rafi — EEE student, Independent University, Bangladesh

