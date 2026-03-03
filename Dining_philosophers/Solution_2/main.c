#include "rtos_tasks.h"
#include "device_mcu_includes.h"
#include "utils_pif.h"
#include "clocking_pif.h"

#include "uart_pif.h"
#include "rng_pif.h"

/**
 * @brief FreeRTOS Dining Philosophers (State Monitor Protocol inspired by Tanenbaum)
 *
 * This project implements "Dining Philosophers" synchronization problem
 * using FreeRTOS EventGroups. The core design is inspired by the solution
 * described by Tanenbaum, but adapted for an RTOS environment using
 * EventGroup.
 *
 * In Tanenbaum's Monitor solution, a philosopher typically asks the neighbor's state
 * (if they are 'Hungry' or 'Thinking') and signals the need for a chopstick.
 *
 * In this implementation, the approach is simplified and optimized for EventGroups:
 * - Instead of asking neighbors, the philosopher directly consults a global state
 * represented by the EventGroup.
 * - The philosopher checks if both neighboring slots are currently free ('1', inverted logic)
 * to determine eligibility to eat.
 *
 * While this method avoids deadlock, it introduces starvation: Philosophers
 * may be repeatedly out-competed by neighbors and receive almost no execution time,
 * as reflected in the runtime statistics.
 *
 * This example demonstrates:
 *  1. Event Group–based resource management for coordinating philosophers
 *     as a flexible alternative to mutex-only solutions.
 *  2. Thread-Safe Logging: Utilizes the printf-stdarg.h library with UART
 *     for debug output, demonstrating safe, concurrent logging protected
 *     by internal mutexes or critical sections.
 *  3. Deferred interrupt handling via Task Notifications to minimize ISR time.
 *  4. RNG peripheral usage to generate random millisecond delays for philosopher timing.
 *
 * How to test:
 *  1. Connect a USB–UART converter to PB7 (TX) and PB6 (RX).
 *  2. Power the NUCLEO board.
 *  3. Plug the USB–UART converter into the PC.
 *  4. Run the example in debug mode.
 *  5. Open the corresponding COM port in any serial terminal at 9600 baud.
 *  6. Send the command "stat" to receive percentage statistics for each philosopher.
 */
int main()
{
    setup_system_clock();
    enable_fpu();
    setup_uart1(9600);
    setup_rng();

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
