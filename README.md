# FreeRTOS Learning Sandbox

This repository contains a collection of experiments and practical tasks developed while learning **FreeRTOS**. The goal of this project is to explore RTOS primitives, task synchronization, and resource management with usage of bare-metal CMSIS of STM32.

## Project Structure

* **`Dining_philosophers/`** - Implementations of Dijkstra's classic "Dining Philosophers" problem. Several solutions using various FreeRTOS mechanisms (semaphores, mutexes, queues) are presented.
* **`Examples/`** – Custom test cases designed to explore the FreeRTOS API, including task notifications, software timers, and scheduler behavior.

> **Note:** Each `main.c` file within the `Examples/` and `Dining_philosophers/` directories contains a Doxygen block explaining the specific logic and objectives of that particular experiment.

## Directory Overview

* **`device/`** – Low-level hardware definitions and debug communication protocols (UART).
* **`Dining_philosophers/`** – Multiple solutions to the synchronization problem, each with its own isolated FreeRTOS configuration.
* **`Examples/`** – Hands-on experiments with RTOS features and STM32 peripherals (ADC, DAC, UART).
* **`FreeRTOS_learning_experiments*.launch`** – Debug configurations for STM32CubeIDE.
* **`generic_utils/`** – Hardware-independent helper functions and logging utilities.
* **`MCU_cores/`** – Vendor-provided CMSIS headers and device-specific register definitions for STM32G4.
* **`Middleware/FreeRTOS/`** – The FreeRTOS kernel source code, including the portable layer for ARM Cortex-M4F.
* **`Startup/`** – Boot code, vector table, and standard C-library syscalls.
* **`stm32g4_sdk/`** – A custom lightweight SDK for system clocking and assembly-optimized utilities.

## Current Environment

* **IDE:** Currently maintained as an STM32CubeIDE project.
* **Hardware:** Targeted for STM32G4 microcontrollers only.

## Roadmap (TODO)

- [ ] Migrate the build system from CubeIDE to CMake.
- [ ] Document specific synchronization patterns used in the examples.