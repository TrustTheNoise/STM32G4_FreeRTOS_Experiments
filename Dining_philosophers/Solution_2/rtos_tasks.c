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

typedef struct {
    uint8_t ucPhilosopherNumber;
    uint32_t ulNeighborSeatsMask;
} PhilosopherProfile_t;

static uint32_t ulTotalMeals;
static uint32_t ulMealsPerPhilosopher[NUM_PHILOSOPHERS];

/**************************************************************************************************/
/*                                                                                                */
/*                                         RTOS resources                                         */
/*                                                                                                */
/**************************************************************************************************/
static EventGroupHandle_t xFreeSeatsEventGroup;
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
    xFreeSeatsEventGroup = xEventGroupCreate();

    xEventGroupSetBits(xFreeSeatsEventGroup, (1 << NUM_PHILOSOPHERS) - 1 );

    printf_safe_init();

    xTaskCreate(prvSerialTerminalTask, "Terminal", 100,
            NULL, 2, &xSerialTerminalNotify);

    char task_name[20];

    // For any Philosopher[i], valid partners start 2 seats away (skipping the neighbor).
    // We initialize the window at 2 and "slide" it forward (+1) for each subsequent philosopher.
    uint8_t ucPhilosopherPrevious = NUM_PHILOSOPHERS - 1;

    for(uint8_t ucPhilosopherNum = 0; ucPhilosopherNum < NUM_PHILOSOPHERS;
            ucPhilosopherNum += 1)
    {
        uint8_t ucPhilosopherNext = ucPhilosopherNum + 1;
        if(ucPhilosopherNext == NUM_PHILOSOPHERS )
        {
            ucPhilosopherNext = 0;
        }

        PhilosopherProfile_t* xProfile =
                (PhilosopherProfile_t*) pvPortMalloc(sizeof(PhilosopherProfile_t));
        xProfile->ucPhilosopherNumber = ucPhilosopherNum;

        xProfile->ulNeighborSeatsMask = ( 1 << ucPhilosopherPrevious | 1 << ucPhilosopherNext );

        sprintf(task_name, "Philosopher %u", ucPhilosopherNum);
        xTaskCreate(prvPhilosopherTask, task_name,
                200, (void*)xProfile, 1, NULL);

        // Shift the window for the next iteration.
        ucPhilosopherPrevious = ucPhilosopherNum;
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
    PhilosopherProfile_t* xPhilosopherProfile = (PhilosopherProfile_t*)parameters;
    const uint32_t ulMyBit = (1 << xPhilosopherProfile->ucPhilosopherNumber);


    while(1)
    {
        // Thinking

        uint32_t ulThinkingDelayMs = (rng_get_value() % 200) + 50;
        uint32_t ulEatingDelayMs = (rng_get_value() % 800) + 100;

        vTaskDelay(pdMS_TO_TICKS(ulThinkingDelayMs));

        // Hungry

        xEventGroupWaitBits(xFreeSeatsEventGroup,
            xPhilosopherProfile->ulNeighborSeatsMask,
            pdFALSE,
            pdTRUE,
            portMAX_DELAY);

        xEventGroupClearBits(xFreeSeatsEventGroup, ulMyBit );

        taskENTER_CRITICAL();
        {
            ulMealsPerPhilosopher[xPhilosopherProfile->ucPhilosopherNumber] += 1;
            ulTotalMeals += 1;
        }
        taskEXIT_CRITICAL();


        // Eating

        printf("Philosopher %u is eating.\r\n", xPhilosopherProfile->ucPhilosopherNumber);
        vTaskDelay(pdMS_TO_TICKS(ulEatingDelayMs));

        printf("Philosopher %u is leaving...\r\n", xPhilosopherProfile->ucPhilosopherNumber);

        xEventGroupSetBits(xFreeSeatsEventGroup, ulMyBit);

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
