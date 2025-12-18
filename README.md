[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21923713)
# ELEC2645 Unit 2 Project

### Electrical Fundamentals Toolkit Calculator

This project is an application written in C that provides a collection of tools for electrical engineering calculations, signal visualisation and communication analysis. It is designed to support learning and experimentation by allowing users to explore engineering concepts through calculations and graphical representation.


## Overview

The application is organised into a menu structure, allowing users to navigate between different toolsets. Each tool focuses on a specific area of engineering and includes robust input validation to ensure reliable operation and meaningful results.

The toolkit is intended for engineering students and communications engineers who want to perform calculations like Ohm's Law or want to test the reliability of their digital communications system.


## Menus

### 1. Electrical Calculator (E-Calc)
The Electrical Calculator provides a set of core electrical engineering concepts, including:
- Ohm’s Law calculations (Voltage, Current, Resistance, and Power)
- dB and power conversions (power ratio to dB)

All calculations include input validation to prevent invalid values.

---

### 2. Signal Analysis & Plotting
This module allows users to generate and visualise signals directly within the terminal using ASCII waveforms.Supported features include:
- Sine, square and triangular waveforms
- User input parameters: Amplitude, frequency (number of cycles), phase and sample count
- Plotting the parameters and waveforms on an ASCII plot

This gives the user an easier way to observe how signal parameters affect a plot without the need for other software.

---

### 3. Communication Tools
The Communication Tools menu focuses on practical system-level analysis and visualisation:
- Signal strength calculator using transmit power and losses
- ASCII-based QAM constellation diagrams (4-QAM, 16-QAM, 64-QAM, 256-QAM)
- Adjustable SNR, channel rotation and Rayleigh fading

A constellation diagram is a graphical representations of modulated symbols used in digital communications to represent how signal modulation is shown on an I/Q plane (where the x-axis represents the in-phase component and the y axis represents the quadrature component of the baseband signal) after being affected by noise and fading. Each point on the diagram is a symbol which corresponds to a certain number of transmitted bits. 

These tools allow users to explore how noise, fading and modulation order affect the behaviour of received symbols.

## Input Validation and Reliability

The application uses an input validation fie to:
- Reject non-numeric inputs
- Enforce valid ranges (e.g. positive-only values where required)
- Prevent division by zero and other invalid operations
- Ensure menu selections are integers only

## Compiling and Running the Code

To compile the code, enter the following into the terminal: gcc main.c e_calc.c input_guard.c signals.c comms.c -o main.out -lm

To run the code, enter ./main.out into the terminal.