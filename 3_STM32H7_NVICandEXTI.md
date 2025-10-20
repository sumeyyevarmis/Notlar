# NVIC
## 1. Overview
For STM32H7 microcontrollers, both CPU1 (Cortex M7) and CPU2 (Cortex M4) cores have their own nested vector interrupt controller (repectivetly NVIC1 and NVIC2), which include the following features;
-  Up to 150 maskable interrupt channels,
-  16 programmable priority levels,
-  Low-latency exceptipn and interrupt handling,
-  Power management control,
-  Implementation of system control register.

## 2. Key Features
The NVIC provides a fast response to interrupt requests, allowing an application to quickly serve incoming events. Most of the peripherals have a unique interrupt vector. The interrupt vector table can also be relocated.

## 3. Exception Entry and Return
The NVIC provides several features for efficient hangling of exception.

When an interrupt is served and a new request with high priority arrives, the new exception van preempt the current one. This is called **nested exception handling.** The previous exception hanfler resumes exception after the higher priority exception is handled.

When an interrupt arrives, the processor first the program context before executing the interrupt hangler. If the processor is performing this contex-saving operation when an interrupt of higher priority arrives, the processor switch directly to hangling the higher-priority interrupt when it is finished saving the program context.

When all of the exception hanflers have been run and no other exception is pending, the processor restores the provious context from the stack and returns to normal application execution.

## 4. Notes
Nesten vector interrupt controller (NVIC) is a method of prioritizing interrupt, improving the MCU's performance and reducing interrupt latency. 

NVIC provides implementation schemes for handling interrupts that occur wher other interrupts are being executed or when the CPU is in the process of restoring its previous state and resuming its suspended process.
