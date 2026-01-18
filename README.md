## Overview 

**Reference Designs** based on 
- Mohamed-U3 PCB V3.1 of hackaBLE https://github.com/Mohamed-U3/ElectronutLabs-hackaBLE
- Reference code https://github.com/agittins/NarfButt\
- uses https://github.com/bouni/kicad-jlcpcb-tools for JLCPCB intergration.
- uses https://raw.githubusercontent.com/CDFER/

**Features**

- Raytac MDBT42 based on Nordic Semiconductor's nRF52832 SOC with chip antenna
- 3 LED's
- Button user configurable, set as RESET by default
- SWD programming headers
- CR2032 power
- Button up
- Button down

**nRF52 IO**
- P0.00 32.768KHz with 12pF load caps
- P0.01 32.768KHz with 12pF load caps
- P0.11 Up push button
- P0.12 Down push button
- P0.17 Red LED via 33 ohm pull up at about 5ma when on. 
- P0.18 Green LED via 10 ohm pull up at about 5ma when on. 
- P0.19 White LED via 10 ohm pull up at about 5ma when on. 
- P0.21 Reset push button via 10k pull up resistor. 
- Ant 0.8pF and 3.9nH filter to PCB trace antenna. 
- XC1 ABM8-32.000MHZ-B2-T3 with 12pF load caps
- XC2 ABM8-32.000MHZ-B2-T3 with 12pF load caps
- VDD 4.7uF, 0.1uF, 0.1uF caps to gnd
- DEC1 0.1uF to Ggnd
- DEC2 No connection
- DEC3 No connection
- DEC4 to DCC via inductors listed below. 
- DCC 10uH to 15nH to DEC4 to 1uF to gnd
- SWD is connected to a dedicated connector with VDD and gnd
- All unused pins are routed to a connector which allows probing with a scope or adding prototype features. 
