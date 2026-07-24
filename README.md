# light-activated-street-light-controller
LM358 + LDR based street light controller with ATmega328P fault detection, simulated in Proteus 8
# Light-Activated Street Light Controller

A light-sensing street light controller built and simulated in **Proteus 8**, using an **LM358 op-amp** as a comparator with an **LDR (Light Dependent Resistor)** as the sensing element. Extended with an **ATmega328P**-based fault detection subsystem to monitor the circuit for faults automatically.

## What it does

- Uses an LDR to detect ambient light levels
- The LM358 op-amp compares the LDR's voltage against a reference threshold
- When ambient light drops below the threshold (e.g. at dusk), the output switches to turn the street light **ON**
- When ambient light rises again (e.g. at dawn), the light turns **OFF**
- An added ATmega328P microcontroller subsystem monitors the circuit for faults (e.g. sensor failure or unexpected behavior), adding a layer of reliability beyond the basic analog design

## Tools used

- **Proteus 8** — circuit design and simulation
- **LM358** — dual op-amp used as a voltage comparator
- **LDR** — light sensing
- **ATmega328P** — fault detection microcontroller subsystem

## Contents of this repo

- `simulation/` — Proteus project file
- `report/` — full IEEE-format project report
- `images/` — circuit screenshots from simulation


## Background

This project was completed as part of coursework (EEE234 – Electronics) at Independent University, Bangladesh (IUB).

## Author

Mir Erfan Kabir Rafi — EEE student, Independent University, Bangladesh
