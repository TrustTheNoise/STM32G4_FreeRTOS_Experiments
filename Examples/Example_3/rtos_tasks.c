#include <string.h>
#include <stdio.h>

#include "utils_pif.h"
#include "printf-stdarg.h"
#include "uart_pif.h"
#include "adc_pif.h"
#include "dac_pif.h"

#include "FreeRTOS.h"
#include "semphr.h"


typedef struct{
    uint32_t ulDacOut[3];
    uint32_t ulAdcOut[3];
} Mutual_Data_t;

Mutual_Data_t xMutualData;

static void prvWriteMutualData(Mutual_Data_t* xMutualDataInstance);
static void prvReadMutualData(Mutual_Data_t* xMutualDataInstance);

/**************************************************************************************************/
/*                                                                                                */
/*                                    RTOS resource variables                                     */
/*                                                                                                */
/**************************************************************************************************/
static void prvOutputTask(DEBUG_UNUSED void* pvParameters);
static void prvSerialTerminalTask(void* pvParameters);
static void prvErrorSignalingTask(void* pvParameters);


static TaskHandle_t xSerialTerminalNotify;
static uint32_t ulReadersCount;

static SemaphoreHandle_t xBarrierMutex;
static SemaphoreHandle_t xDataAccessSemaphore;

void vRtosSetup(void)
{
    printf_safe_init();

    xBarrierMutex = xSemaphoreCreateMutex();
    xDataAccessSemaphore = xSemaphoreCreateBinary();


    vTaskStartScheduler();
}

/**************************************************************************************************/
/*                                                                                                */
/*                                           RTOS tasks                                           */
/*                                                                                                */
/**************************************************************************************************/

static void prvOutputTask(DEBUG_UNUSED void* pvParameters)
{
    while(1)
    {
        Mutual_Data_t xReadData;

        TickType_t xTickNow = xTaskGetTickCount();

        vReadMutualData(&xReadData);

        printf("[%u]", xTickNow );

        for(uint8_t i=0; i<=sizeof(xMutualData.ulDacOut); i += 1)
        {
            float fDacVoltage = ((float)xReadData.ulDacOut[i] / 0xFFFF) * 3.3;
            float fAdcVoltage = ((float)xReadData.ulAdcOut[i] / 0xFFFF) * 3.3;

            float fDifferenceVoltage = fDacVoltage - fAdcVoltage;
            if(fDifferenceVoltage < 0)
            {
                fDifferenceVoltage = -1.0f * fDifferenceVoltage;
            }

            uint32_t ulIntegerPart = (uint32_t) fDifferenceVoltage;
            uint32_t ulFractionalPart = (uint32_t)((fDifferenceVoltage
                                                 - ulIntegerPart) * 100000);
            printf("channel %u: %u.%u", i, ulIntegerPart, ulFractionalPart);
        }

        printf("\r\n");
    }
}


static void prvErrorSignalingTask(void* pvParameters)
{
    uint8_t ucGpioNumber = (uint32_t) pvParameters;

    uint32_t ulGpioBssrSet = (0b1 << (GPIO_BSRR_BS0_Pos + ucGpioNumber));
    uint32_t ulGpioBssrClear = (0b1 << (GPIO_BSRR_BR0_Pos + ucGpioNumber));

    while(1)
    {
        Mutual_Data_t xReadData;

        prvReadMutualData(&xReadData);

        float fDacVoltage = ((float)xReadData.ulDacOut[ulGpioBssrSet] / 0xFFFF) * 3.3;
        float fAdcVoltage = ((float)xReadData.ulAdcOut[ulGpioBssrSet] / 0xFFFF) * 3.3;

        float fDifferenceVoltage = fDacVoltage - fAdcVoltage;
        if(fDifferenceVoltage < 0)
        {
            fDifferenceVoltage = -1.0f * fDifferenceVoltage;
        }

        if(fDifferenceVoltage > 0.00001)
        {
            GPIOB->BSRR = ulGpioBssrSet;
        }else
        {
            GPIOB->BSRR = ulGpioBssrClear;
        }
    }
}


static void prvSerialTerminalTask(DEBUG_UNUSED void* pvParameters)
{

    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        const uint16_t received_dma_size = uart1_get_received_message_len();
        uint8_t* received_buffer = uart1_get_receive_buffer();


        if (memcmp(received_buffer, "inc", 3) == 0)
        {

            if(memcmp(&received_buffer[3], "1\r\n", 3) == 0)
            {

            }
            if(memcmp(&received_buffer[3], "2\r\n", 3) == 0)
            {

            }
            if(memcmp(&received_buffer[3], "3\r\n", 3) == 0)
            {

            }
        }else
        {
            printf("%.*s\n", received_dma_size, received_buffer);
        }

        // Reset RX DMA after every timeout to make DMA write back from 0
        uart1_reset_rx_dma();

    }
}


static void prvDecreseDacTask(DEBUG_UNUSED void* pvParameters)
{
    while()
}


static void prvWriteMutualData(Mutual_Data_t* xMutualDataInstance)
{
    xSemaphoreTake(xBarrierMutex, portMAX_DELAY);
    xSemaphoreTake(xDataAccessSemaphore, portMAX_DELAY);

    for(uint8_t i = 0; i < 3; i+=1)
    {
        xMutualData.ulAdcOut[i] = xMutualDataInstance->ulAdcOut[i];
        xMutualData.ulDacOut[i] = xMutualDataInstance->ulDacOut[i];
    }

    xSemaphoreGive(xDataAccessSemaphore);
    xSemaphoreGive(xBarrierMutex);
}


static void prvReadMutualData(Mutual_Data_t* xMutualDataInstance)
{
    xSemaphoreTake(xBarrierMutex, portMAX_DELAY);
    xSemaphoreGive(xBarrierMutex);

    taskENTER_CRITICAL();
        ulReadersCount += 1;
        if(ulReadersCount == 1)
        {
            xSemaphoreTake(xDataAccessSemaphore, portMAX_DELAY);
        }
    taskEXIT_CRITICAL();

    for(uint8_t i = 0; i < 3; i+=1)
    {
        xMutualDataInstance->ulAdcOut[i] = xMutualData.ulAdcOut[i];
        xMutualDataInstance->ulDacOut[i] = xMutualData.ulDacOut[i];
    }


    taskENTER_CRITICAL();
        ulReadersCount -= 1;
        if(ulReadersCount == 0)
        {
            xSemaphoreGive(xDataAccessSemaphore);
        }
    taskENTER_CRITICAL();

    xSemaphoreGive(xDataAccessSemaphore);
}

/**************************************************************************************************/
/*                                                                                                */
/*                                       Interrupt Handlers                                       */
/*                                                                                                */
/**************************************************************************************************/
void USART1_IRQHandler()
{
    // UART data receive sections
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    if( USART1->ISR & USART_ISR_RTOF ) // UART message received
    {
        USART1->ICR |= USART_ICR_RTOCF; // Clear the timeout interrupt flag

        vTaskNotifyGiveFromISR(xSerialTerminalNotify,
                &pxHigherPriorityTaskWoken);

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


void ADC1_2_IRQHandler(void)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if ( ADC1->ISR & ADC_ISR_JEOS ) // End of injected conversions sequence
    {
        ADC1->ISR |= ADC_ISR_JEOS;

    }

    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}
