# Description of the DWT_Init Function
This function enables the cycle counter of the DWT (Data Watchpoint and Trace) unit found in Cortex-M series processors. In other words, it resets and starts a hardware counter that counts CPU clock cycles-allowing you to perform very high-resolution timing and preformance measurements.
```
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // DWT enable
    DWT->CYCCNT = 0;                                // reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // start counter
}
```
- ````CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;````
It sets the **TRCENA** bit in **DEMCR (Debug Exception and Monitor Control Register)**. This bit enables the operation of core tracing units such as Trace/DWT/ITM. In other words, to use the DWT, we first need to enable the trace insfastructure.

- ``DWT->CYCCNT = 0;``
It resets the cycle counter. **CYCCNT** represents a 32-bit counter; here, we clear it to start a fresh measurement.

- ``DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;``
It starts the counter by setting the **CYCCBTENA (cycle counter enable)** bit in the DWT control register.

## What is it used for?
The DWT cycle counter is usd to measure how many CPU cycles a piece of code consumes.


# Test 1
## 1. Purpose
Observing how many cycles it take to read data from RAM.
```
#define SIZE 10000

// Flash da saklanan veri
const uint32_t flash_array[SIZE] = {1};

// RAM de saklanan veri
uint32_t ram_array[SIZE];

// DWT cycle counter init
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // DWT enable
    DWT->CYCCNT = 0;                                // reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // start counter
}


int main(void){
    DWT_Init();

    uint32_t start = DWT_GetCycles();


    uint32_t sum = 0;

    // RAM den okuma
    for (int i = 0; i < SIZE; i++) {
         sum += ram_array[i];
    }
    uint32_t end = DWT_GetCycles();

    uint32_t cycles = end - start;
}
```

![first](doc/1_1-Lab-1-Cache_image1.png)

# Test 2
## 1. Purpose
Observing how many cycles it take to read data from FLASH.
```
#define SIZE 10000

// Flash da saklanan veri
const uint32_t flash_array[SIZE] = {1};

// RAM de saklanan veri
uint32_t ram_array[SIZE];

// DWT cycle counter init
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // DWT enable
    DWT->CYCCNT = 0;                                // reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // start counter
}


int main(void){
    DWT_Init();

    uint32_t start = DWT_GetCycles();


    uint32_t sum = 0;

    // FLASH’tan okuma
    for (int i = 0; i < SIZE; i++) {
	    sum += flash_array[i];
    }
    uint32_t end = DWT_GetCycles();

    uint32_t cycles = end - start;
}
```

![second](doc/1_1-Lab-1-Cache_image2.png)


# Test 3

```
#define SIZE 1024

// Flash da saklanan veri
const uint32_t flash_array[SIZE] = {1,2,3,4};

// RAM de saklanan veri
uint32_t ram_array[SIZE] = {1,2,3,4};

// DWT cycle counter init
void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // DWT enable
    DWT->CYCCNT = 0;                                // reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // start counter
}
uint32_t measure_flash(void) {
    volatile uint32_t sum = 0;
    uint32_t start = DWT->CYCCNT;
    for (int i = 0; i < SIZE; i++) {
        sum += flash_array[i];
    }
    uint32_t end = DWT->CYCCNT;
    return end - start;
}

uint32_t measure_ram(void) {
    volatile uint32_t sum = 0;
    uint32_t start = DWT->CYCCNT;
    for (int i = 0; i < SIZE; i++) {
        sum += ram_array[i];
    }
    uint32_t end = DWT->CYCCNT;
    return end - start;
}

void Cache_Disable(void) {
    SCB_DisableICache();
    SCB_DisableDCache();
}

void Cache_Enable(void) {
    SCB_EnableICache();
    SCB_EnableDCache();
}

int main(void){
    DWT_Init();


    Cache_Enable();
    int cacheEnableFlash = measure_flash();
    int cacheEnableRam =measure_ram();


    Cache_Disable();
    int cacheDisableFlash = measure_flash();
    int cacheDisableRam =measure_ram();
}
```

![third](doc/1_1-Lab-1-Cache_image3.png)

![fourth](doc/1_1-Lab-1-Cache_image4.png)

![fifth](doc/1_1-Lab-1-Cache_image5.png)