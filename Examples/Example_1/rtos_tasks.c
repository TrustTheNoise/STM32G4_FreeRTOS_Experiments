#include "debug_utils.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"

typedef enum
{
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
} ButtonNumber_t;

typedef struct
{
    ButtonNumber_t xButtonNum;
    uint8_t ucButtonState;
} ButtonEvent_t;

QueueHandle_t xButtonEventQueue = NULL;
TimerHandle_t xTimerBlinkLed = NULL;

static void vEventManagerTask(DEBUG_UNUSED void* pvParameters);
static void vButtonPollingTask(DEBUG_UNUSED void* pvParameters);
static void prvBlinkingLedTimerCallback(DEBUG_UNUSED TimerHandle_t xTimer);


void vRtosSetup(void)
{
    xButtonEventQueue = xQueueCreate(1, sizeof(ButtonEvent_t));

    xTimerBlinkLed = xTimerCreate("BlinkLed", pdMS_TO_TICKS(300), pdTRUE,
            NULL, prvBlinkingLedTimerCallback);

    xTaskCreate(vEventManagerTask, "EventManager", 100, NULL,
            3, NULL);
    xTaskCreate(vButtonPollingTask, "ButtonPolling", 100, NULL,
            1, NULL);

    xTimerStart(xTimerBlinkLed, 0);

    vTaskStartScheduler();
}


static void vEventManagerTask(DEBUG_UNUSED void* pvParameters)
{
    ButtonEvent_t xGetButtonEvent;

    while(1)
    {
        xQueueReceive(xButtonEventQueue, &xGetButtonEvent, portMAX_DELAY);

        switch(xGetButtonEvent.xButtonNum)
        {
        case BUTTON_1:
            if(xGetButtonEvent.ucButtonState)
            {
                GPIOC->BSRR = GPIO_BSRR_BS13;
            }else
            {
                GPIOC->BSRR = GPIO_BSRR_BR13;
            }
            break;
        case BUTTON_2:
            if(xGetButtonEvent.ucButtonState)
            {
                GPIOC->BSRR = GPIO_BSRR_BS2;
            }else
            {
                GPIOC->BSRR = GPIO_BSRR_BR2;
            }
            break;
        case BUTTON_3:
            if(xGetButtonEvent.ucButtonState)
            {
                GPIOC->BSRR = GPIO_BSRR_BS3;
            }else
            {
                GPIOC->BSRR = GPIO_BSRR_BR3;
            }
            break;
        default:
            break;
        }
    }
}


static void vButtonPollingTask(DEBUG_UNUSED void* pvParameters)
{

    uint8_t pucPreviousButtonState[] = {0, 0, 0};
    ButtonEvent_t xButtonEvent;

    while(1)
    {
        for(uint8_t ucButtonIndex = 0; ucButtonIndex < 3; ucButtonIndex+=1)
        {
            uint8_t ucButtonState = (GPIOB->IDR &
                       (0b1U << (GPIO_IDR_ID3_Pos + ucButtonIndex))) != 0;
            if(ucButtonState != pucPreviousButtonState[ucButtonIndex])
            {
                vTaskDelay(pdMS_TO_TICKS(20)); // anti debounce delay

                uint8_t ucNewState = (GPIOB->IDR &
                           (0b1U << (GPIO_IDR_ID3_Pos + ucButtonIndex))) != 0;

                if( ucButtonState == ucNewState )
                {
                    xButtonEvent.xButtonNum = (ButtonNumber_t)ucButtonIndex;
                    xButtonEvent.ucButtonState = ucButtonState;

                    xQueueSend(xButtonEventQueue, &xButtonEvent, portMAX_DELAY);

                    pucPreviousButtonState[ucButtonIndex] = ucButtonState;
                }
            }
        }
    }
}

static void prvBlinkingLedTimerCallback(DEBUG_UNUSED TimerHandle_t xTimer)
{
    GPIOC->ODR ^= GPIO_ODR_OD0;
}
