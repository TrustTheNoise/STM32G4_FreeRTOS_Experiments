#include "rtos_tasks.h"
#include "device_mcu_includes.h"
#include "dac_pif.h"
#include "uart_pif.h"
#include "adc_pif.h"
#include "clocking_pif.h"

/**
 * @brief FreeRTOS ADC Sampling and Averaging Example
 *
 * This project demonstrates the use of FreeRTOS tasks, semaphores, and ISR-to-task
 * notifications for processing ADC data in real time on an STM32 microcontroller.
 *
 * Scenario:
 * 1. A hardware timer triggers an ISR to sample the ADC ten times per second (every 100 ms).
 * 2. The ISR stores each ADC sample in a buffer.
 * 3. Once 10 samples are collected (1 second), the ISR notifies Task A to process the data.
 * 4. Task A computes the average of the 10 ADC samples and stores the result in a global float variable.
 *    Note: the float variable cannot be read or written in a single CPU instruction,
 *    so proper synchronization (e.g., semaphore or task notification) is required.
 * 5. Task B handles UART communication:
 *    - Echoes any received character.
 *    - If the command "avg" is received, Task B prints the value of the averaged ADC samples.
 *
 * FreeRTOS concepts exercised:
 * - Task creation and management
 * - ISR-to-task notifications or semaphores
 * - Synchronization between ISR and tasks
 * - Shared resource protection (global float variable)
 * - Queue or buffer usage for ADC samples
 */
int main()
{
    setup_system_clock();
    setup_uart1(9600);
    setup_adc();
    setup_dac();

    adc_start_injected_sampling(); // ADSTRT/JADSTART must be 1 to enable triggered measurements

    vRtosSetup();

    while(1) {}

    return 0;
}

//  putchar is the only external dependency for printf-stdarg.c
int putchar(int c)
{
    uart1_send_byte((uint8_t)c);

    return c;
}
