# Simple Calculator
The implementation of Simple Calculator with abstraction layers using the ATmega32 microcontroller.

## Introduction

Developing a simple calculator for the simplest mathematical operations (addition, subtraction, multiplication, division) with order of operations.

## Hardware

* ATmega32 Microcontroller
* Keypad
* 16x4 LCD

## Software Requirements

* Downloaded project code
* GNU Compiler Collection (GCC) for AVR (https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)
* Optional: AVR Studio (for debugging) (https://atmel-studio.software.informer.com/7.0/)

## Prerequisites

* Downloaded and installed Eclipse IDE for Embedded C/C++ Developers (https://www.eclipse.org/downloads/packages/release/2020-12/r/eclipse-ide-embedded-cc-developers)
* Working knowledge of C programming and basic understanding of embedded systems and abstraction layers

## Setting Up Eclipse

1. **Install Eclipse Package:** Download and install the "Eclipse IDE for Embedded C/C++ Developers" package from the Eclipse website.
2. **Create New Project:** Launch Eclipse and select "C/C++ Project" under "New" -> "Project". Choose an appropriate name for your weather station project.
3. **Configure Toolchain:** In the project properties, go to "C/C++ Build" -> "Settings" and define the path to your AVR GCC installation under "Toolchains".
4. **Add Source Files:** Import the downloaded project code (C files and header files) into your Eclipse workspace. These files will typically be organized using a directory structure for the abstraction layers (HAL, MCAL, APP) and any additional helper files.
5. **Configure Build Options:** In the project properties, go to "C/C++ Build" -> "Settings" and configure build options like target device (ATmega328P) and clock frequency.

## Software Abstraction Layers

This project leverages software abstraction layers to improve code modularity, maintainability, and portability. Here's a breakdown of the typical abstraction layers:

* **Hardware Abstraction Layer (HAL):** (HAL/)
  * Keypad Driver
  * LCD Driver
* **Middleware Abstraction Layer (MCAL):** (MCAL/)
  * GPIO Driver
* **Application Layer (APP):** (APP/)

## Benefits of Abstraction Layers

* Modular and maintainable code.
* Easier to modify functionalities within specific layers.
* Improved code reusability for common functions.

  
