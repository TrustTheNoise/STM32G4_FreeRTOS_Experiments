#include "rtos_tasks.h"
#include "device_mcu_includes.h"
#include "utils_pif.h"
#include "clocking_pif.h"

#include "uart_pif.h"
#include "rng_pif.h"

/**
 * @brief FreeRTOS Dining Philosophers (Resource Hierarchy Solution)
 *
 * This project implements the classic "Dining Philosophers" problem
 * using FreeRTOS Mutexes, following the Resource Hierarchy strategy
 * (Dijkstra's solution).
 *
 * Each philosopher always acquires the lower-indexed chopstick first,
 * then the higher one. In practice, this means that all philosophers
 * except the last one pick up their chopsticks in the normal order,
 * while the last philosopher picks them up in the reverse order.
 *
 * As with all mutex-based versions, starvation is still possible if scheduling
 * repeatedly favors certain philosophers.
 *
 * This example demonstrates:
 *  1. Mutex resource management for coordinating philosophers
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
