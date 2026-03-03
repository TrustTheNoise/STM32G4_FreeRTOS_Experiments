#include <string.h>
#include <stdio.h>

#include "utils_pif.h"
#include "rng_pif.h"
#include "printf-stdarg.h"
#include "uart_pif.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"


/**************************************************************************************************/
/*                                                                                                */
/*                                     Application resources                                      */
/*                                                                                                */
/**************************************************************************************************/

#define NUM_PHILOSOPHERS       5

static uint32_t ulTotalMeals;
static uint32_t ulMealsPerPhilosopher[NUM_PHILOSOPHERS];

/**************************************************************************************************/
/*                                                                                                */
/*                                         RTOS resources                                         */
/*                                                                                                */
/**************************************************************************************************/
static SemaphoreHandle_t xSeatsSemaphore;
static SemaphoreHandle_t xChopsticks[NUM_PHILOSOPHERS];

static TaskHandle_t xSerialTerminalNotify = NULL;

static void prvSerialTerminalTask(DEBUG_UNUSED void* parameters);
static void prvPhilosopherTask(void *parameters);


/**************************************************************************************************/
/*                                                                                                */
/*                                  Global functions definitions                                  */
/*                                                                                                */
/**************************************************************************************************/

void vRtosSetup(void)
{
    xSeatsSemaphore = xSemaphoreCreateCounting(NUM_PHILOSOPHERS - 1, NUM_PHILOSOPHERS - 1);

    printf_safe_init();

    xTaskCreate(prvSerialTerminalTask, "Terminal", 100,
            NULL, 2, &xSerialTerminalNotify);

    char task_name[20];

    for(uintptr_t ucPhilosopherNum = 0; ucPhilosopherNum < NUM_PHILOSOPHERS;
            ucPhilosopherNum += 1)
    {

        xChopsticks[ucPhilosopherNum] = xSemaphoreCreateMutex();

        sprintf(task_name, "Philosopher %u", ucPhilosopherNum);
        xTaskCreate(prvPhilosopherTask, task_name,
                200, (void*)ucPhilosopherNum, 1, NULL);
    }

    vTaskStartScheduler();
}

/**************************************************************************************************/
/*                                                                                                */
/*                                           RTOS tasks                                           */
/*                                                                                                */
/**************************************************************************************************/
static void prvSerialTerminalTask(DEBUG_UNUSED void* parameters)
{
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        const uint16_t received_dma_size = uart1_get_received_message_len();
        uint8_t* received_buffer = uart1_get_receive_buffer();


        if (memcmp(received_buffer, "stat\r\n", received_dma_size) == 0)
        {
            printf("********Statistic of starvation*********\r\n");

            for(uint8_t ucPhilosopherNum = 0; ucPhilosopherNum < NUM_PHILOSOPHERS; ucPhilosopherNum+=1 )
            {
                float fPhilosopherMealsPercent = ((float)ulMealsPerPhilosopher[ucPhilosopherNum]
                                                                             / ulTotalMeals) * 100.0f;
                uint32_t ulIntegerPart = (uint32_t) fPhilosopherMealsPercent;
                uint32_t ulFractionalPart = (uint32_t)((fPhilosopherMealsPercent
                                                     - ulIntegerPart) * 100);

                printf("Philosopher %u ate for %u.%u%% of the total meals\r\n",
                       ucPhilosopherNum, ulIntegerPart, ulFractionalPart);
            }

            printf("****************************************\r\n");

        }

        // Reset RX DMA after every timeout to make DMA write back from 0
        uart1_reset_rx_dma();

    }

}


static void prvPhilosopherTask(void *parameters)
{
    uint8_t ucPhilosopherNum = (uintptr_t)parameters;

    uint8_t ucLeftChopstick = ucPhilosopherNum;
    uint8_t ucRightChopstick = ucPhilosopherNum + 1;
    if(ucRightChopstick == NUM_PHILOSOPHERS)
    {
        ucRightChopstick = 0;
    }

    while(1)
    {
        // thinking

        uint32_t ulThinkingDelayMs = (rng_get_value() % 200) + 50;

        vTaskDelay(pdMS_TO_TICKS(ulThinkingDelayMs));

        // hungry

        uint32_t ulEatingDelayMs = (rng_get_value() % 800) + 100;

        xSemaphoreTake(xSeatsSemaphore, portMAX_DELAY);

        xSemaphoreTake(xChopsticks[ucLeftChopstick], portMAX_DELAY);
        xSemaphoreTake(xChopsticks[ucRightChopstick], portMAX_DELAY);

        taskENTER_CRITICAL();
        {
            ulMealsPerPhilosopher[ucPhilosopherNum] += 1;
            ulTotalMeals += 1;
        }
        taskEXIT_CRITICAL();


        // eating

        printf("Philosopher %u is eating.\r\n", ucPhilosopherNum);
        vTaskDelay(pdMS_TO_TICKS(ulEatingDelayMs));

        printf("Philosopher %u is leaving...\r\n", ucPhilosopherNum);
        xSemaphoreGive(xChopsticks[ucLeftChopstick]);
        xSemaphoreGive(xChopsticks[ucRightChopstick]);
        xSemaphoreGive(xSeatsSemaphore);
    }
}

/**************************************************************************************************/
/*                                                                                                */
/*                                       Interrupt Handlers                                       */
/*                                                                                                */
/**************************************************************************************************/

void USART1_IRQHandler()
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if( USART1->ISR & USART_ISR_RTOF ) // UART message received
    {
        USART1->ICR |= USART_ICR_RTOCF; // Clear the timeout interrupt flag

        vTaskNotifyGiveFromISR(xSerialTerminalNotify, &pxHigherPriorityTaskWoken);
    }

    // UART error handling section

    if (USART1->ISR & USART_ISR_ORE) // Overrun error detected;
    {
        USART1->ICR = USART_ICR_ORECF;
        LOG_ERROR(9001);
    }

    if (USART1->ISR & USART_ISR_NE) // Noise error detected;
    {
        USART1->ICR = USART_ICR_NECF;
        LOG_ERROR(9002);
    }

    if(USART1->ISR & USART_ISR_FE) // Framing error detected;
    {
        USART1->ICR = USART_ICR_FECF;
        LOG_ERROR(9003);
    }

    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}
