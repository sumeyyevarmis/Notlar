
# Dual ARM Cortex Cores
The industrial STM32H745xI/G devices embed two ARM cores, a Cortex-M7 and a Cortex-M4. The Cortex-M4 offers optimal performance for real-time applications while the Cortex-M7 core can execute high-performance tasks in parallel.

The two cores belog to separate power domanins. This allows designing guradual high-power efficiency solutions in combination with the low-power modes already available on all STM32 microcontrollers.

## ARM Cortex-M7 with FPU
The Arm Cortex-M7 with double-precision FPU processor is the latest generation of Arm processors for embedded systems. Delivering outstanding computational performance and low interrupt latency.

The Cortex-M7 processor is a highly efficient high-performance featuring:
- Six-stage dual-issuse pipeline
- Dynamic branch prediction
- Harvard architecture with L1 caches (16 Kbytes of I-cache and 16 Kbytes of D-cache)
- 64-bit ACI interface
- 64-bit ITCM interface
- 2x32-bit DTCM interface

The following memory interfaces anre supported:
- Separete Instruction and Data buses (Harvard Architecture) to optimize CPU latency.
- Tightly Coupled Memory (TCM) interface designed for fast and determisitic SRAM accesses.
- AXI Bus interface to optimize Brust transfers.
- Dedicated low-latency AHB-Lite peripheral bus (AHBP) to connect to peripherals.

The processor supports a set of DSP instructions which allow efficient signal processing and complex algorithm execution.

It also supports single and double precision FPU (floating point unit).

## ARM Cortex-M4 with FPU
The Arm Cortex-M4 processor is a high-performance embedded processor which supports DSP instructions. Delivering outstanding computational performance and low interrupt latency.

The Arm Cortex-M4 processor is a highly efficient MCU featuring:
- 3-stage pipeline with branch prediction
- Harvard architecture
- 32-bit System (S-BUS) interface
- 32-bit I-BUS interface
- 32-bit D-BUS interface

The Arm Cortex-M4 processor also features a dedicated harware adaptiive real-time accelerator (ART Accelerator). This is an instruction cache memory composed of sixty-four 256-bit lines, a 256-bit cachee buffer connected to the 64-bit AXXI interface and 32-bit interface for non-cacheable accesses.

# Topic Questions

## 1. Cache ?

### a. Cache Memory in Computer Organization
Cache memory is a small, high-speed storafe area in a computer. It stores copies of the data from frequently used main memory locations. There are various independent caches in a CPU, which store instructions and data.

- The most impoertant use cache memory is that it is used to reduce the average time to access data from main memory.
- The concept of cache works because there exists locality of reference (the same items or nearby items are more likely to be accessed next) in processes.

By storing this information closer to the CPU, cache memory helps speed up the overall processing time. Cache memory is much faster than the main memory (RAM). When the CPU needs data, it first checks the cache. If the data is there, the CPU can access it quickly. If not, it must fetch the data from the slower main memory.

### b. Characteristics of Cache Memory
- Extremely fast memory type that acts as a buffer between RAM and the CPU.
- Holds frequently requested data and instructions, ensuring that they are immendiately available to the CPU when needed.
- Constlier than main memory or disk memory but more economical than CPU registers.
- Used to speed up processing and synchronize with the high-speed (CPU)

![CPU - Cache - Primary/Secondary Memory](doc/1_1-CacheMemory.png)


### c. Levels of Memory
#### Level 1 or Register:
It is type of memory in which data is stored and accepted that are immediately stored in the CPU. The most commonly used register is Accumulator, Program counter, Address register etc.

#### Level 2 or Cache Memory:
It is the fastest memory that has faster access time where data is temporarily stored for faster access.

#### Level 3 or Main Memory:
It is the memory on which the computer works currently. It is small in size and once power is off data no longer stays in this memory.


#### Level 4 or Secondary Memory: 
It is external memory that is not as fast as the main memory but data stays permanently in this memory.

### d. Advantages
- Cache Memory is faster in comparison to mian memory and secondary memory.
- Programs stored by Cache Memory can be executed in less time.
- The data access time of Cache Memory is less than that of main memory.
- Cache Memory stored data and insturctions that are regularly used by the CPU, therefore it increases the performance of the CPU.

### e. Disadvantages
- Cache Memory is costlier than primary memory and secondary memory.
- Data is stored on temporary basis in Cache Memory.
- Whenever the system turned off, data and instructions stored in cache memory get destroyed.
- The high cost of cache memory increases the price of the Computer System.