#include <string.h>
#include <stdio.h>

#include "utils_pif.h"
#include "printf-stdarg.h"
#include "uart_pif.h"
#include "adc_pif.h"
#include "dac_pif.h"

#include "FreeRTOS.h"
#include "semphr.h"


static const uint32_t* pulInjectedBuffer;

#define ADC_SAMPLES_QUANTITY       10
UBaseType_t uxADCSampleBufferA[ADC_SAMPLES_QUANTITY];
UBaseType_t uxADCSampleBufferB[ADC_SAMPLES_QUANTITY];
UBaseType_t *puxADCSampleCurrentBuffer = NULL;
UBaseType_t *puxADCSampleFullBuffer = NULL;

QueueHandle_t xMutexADCAverage = NULL;

float fADCAverage;

/**************************************************************************************************/
/*                                                                                                */
/*                                    RTOS resource variables                                     */
/*                                                                                                */
/**************************************************************************************************/
TaskHandle_t xOverasmpleADCNotify = NULL;
TaskHandle_t xSerialTerminalNotify = NULL;

static void pvOversampleADCTask(void* pvParameters);
static void pvSerialTerminalTask(void* pvParameters);

void vRtosSetup(void)
{
    printf_safe_init();

    puxADCSampleCurrentBuffer = uxADCSampleBufferA;
    pulInjectedBuffer = adc_get_injected_measurements();

    xMutexADCAverage = xSemaphoreCreateMutex();

    xTaskCreate(pvOversampleADCTask, "average", 1024,
            NULL, 2, &xOverasmpleADCNotify);

    xTaskCreate(pvSerialTerminalTask, "terminal", 1024,
            NULL, 1, &xSerialTerminalNotify);


    vTaskStartScheduler();
}

/**************************************************************************************************/
/*                                                                                                */
/*                                           RTOS tasks                                           */
/*                                                                                                */
/**************************************************************************************************/

static void pvOversampleADCTask(DEBUG_UNUSED void* pvParameters)
{

    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        xSemaphoreTake(xMutexADCAverage, portMAX_DELAY);
        {
            fADCAverage = 0;

            for(uint8_t ucSampleIndex = 0; ucSampleIndex < ADC_SAMPLES_QUANTITY;
                    ucSampleIndex+=1)
            {
                fADCAverage += (float)puxADCSampleFullBuffer[ucSampleIndex];
            }

            fADCAverage = fADCAverage / (float)ADC_SAMPLES_QUANTITY;
        }
        xSemaphoreGive(xMutexADCAverage);
    }
}

static void pvSerialTerminalTask(DEBUG_UNUSED void* pvParameters)
{

    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        const uint16_t received_dma_size = uart1_get_received_message_len();
        uint8_t* received_buffer = uart1_get_receive_buffer();


        if (memcmp(received_buffer, "avg\r\n", received_dma_size) == 0)
        {

            vTaskSuspendAll();
                float fAverage = fADCAverage;
            xTaskResumeAll();

            uint32_t ulIntegerPart = (uint32_t) fAverage;
            uint32_t ulFractionalPart = (uint32_t)((fAverage
                                                 - ulIntegerPart) * 100);

            printf("average = %u.%u%%\r\n", ulIntegerPart, ulFractionalPart);


            printf("average = %f\r\n", fADCAverage);
        }else
        {
            printf("%.*s\n", received_dma_size, received_buffer);
        }

        // Reset RX DMA after every timeout to make DMA write back from 0
        uart1_reset_rx_dma();

    }
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
    static uint8_t ucADCConvertionsIndex = 0;
    static uint8_t ucDACDivider = 1;

    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if ( ADC1->ISR & ADC_ISR_JEOS ) // End of injected conversions sequence
    {
        ADC1->ISR |= ADC_ISR_JEOS;


        puxADCSampleCurrentBuffer[ ucADCConvertionsIndex ] = pulInjectedBuffer[0];

        dac_set_channel_1_value(ucDACDivider);

        ucADCConvertionsIndex += 1;
        ucDACDivider += 1;

        if(ucADCConvertionsIndex == ADC_SAMPLES_QUANTITY)
        {
            puxADCSampleFullBuffer = puxADCSampleCurrentBuffer;

            if(puxADCSampleCurrentBuffer == uxADCSampleBufferA)
            {
                puxADCSampleCurrentBuffer = uxADCSampleBufferB;
            }else
            {
                puxADCSampleCurrentBuffer = uxADCSampleBufferA;
            }

            ucADCConvertionsIndex = 0;
            ucDACDivider = 1;

            vTaskNotifyGiveFromISR(xOverasmpleADCNotify,
                    &pxHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}
