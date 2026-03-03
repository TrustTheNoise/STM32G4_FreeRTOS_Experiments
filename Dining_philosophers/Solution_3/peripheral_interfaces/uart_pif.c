#include "uart_pif.h"

/**************************************************************************************************/
/*                                                                                                */
/*                                     Peripheral definitions                                     */
/*                                                                                                */
/**************************************************************************************************/
/*!
 * This code uses:
 *
 * GPIO:
 *  PB6 - USART1 TX
 *  PB7 - USART1 RX
 *
 * USART:
 *  USART1
 *
 * DMA:
 *  DMA1 channel 1 - USART1 RX
 *  DMA1 channel 2 - USART1 TX
 *
 * DMA MUX:
 *  DMA_MUX1 channel 0 - DMA1 channel 1
 *  DMA_MUX1 channel 1 - DMA1 channel 2
 *
 * Interrupts:
 *  USART1_IRQn
 */

/**************************************************************************************************/
/*                                                                                                */
/*                                        Local definitions                                       */
/*                                                                                                */
/**************************************************************************************************/

// Simplification definitions for DMA channels. Only used to improve readability.
//  If other channels are needed. All "@update-type: manual" functions must be updated manually!
#define UART_RX_DMA_CH                      DMA1_Channel1
#define UART_RX_DMAMUX_CH                   DMAMUX1_Channel0

#define UART_TX_DMA_CH                      DMA1_Channel2
#define UART_TX_DMAMUX_CH                   DMAMUX1_Channel1

/**************************************************************************************************/
/*                                                                                                */
/*                                  Static variables declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

uint8_t dma_buffer[UART_1_BUFFER_SIZE] = {0};

/**************************************************************************************************/
/*                                                                                                */
/*                                  Static functions declarations                                 */
/*                                                                                                */
/**************************************************************************************************/

static inline void setup_uart_gpio( void );

static inline void setup_uart_peripheral( uint32_t desired_uart_baud_rate );

static inline uint16_t setup_uart_dma( void );

/**************************************************************************************************/
/*                                                                                                */
/*                                Global functions implementations                                */
/*                                                                                                */
/**************************************************************************************************/

/*!
 * @brief Sets up UART to a provided baud_rate
 *
 * @update-type: #none
 */
void setup_uart1( uint32_t desired_uart_baud_rate )
{
    setup_uart_gpio();

    setup_uart_peripheral( desired_uart_baud_rate );

    (void)LOG_ERROR( setup_uart_dma() );
}

/**************************************************************************************************/
/*                                                                                                */
/*                       Simple communication UART functions implementations                      */
/*                                                                                                */
/**************************************************************************************************/

/**
 * @brief Simplest UART send function for a single byte
 *
 * @update-type: #auto
 *
 * Works both with and without FIFO enabled
 */
void uart1_send_byte( const uint8_t byte_to_send )
{
    while ( (USART1->ISR & USART_ISR_TC) != USART_ISR_TC ){} // Check that the last message was sent
    USART1->TDR = byte_to_send;
}


/**
 * @brief Sends break message to the USART line right after the ongoing transmission is complete
 *
 * @update-type: #auto
 *
 * break message is a message full of "0" with 2 stop bits at the end.
 */
void uart1_send_break_charactes( void )
{
    USART1->RQR |= USART_RQR_SBKRQ;
}

/**
 * @brief Resets DMA, sets it back up again and sends the target message.
 *
 * @update-type: #auto
 *
 * @note The message must live long enough to be copied by the DMA! If message is corrupted so will
 * be the transfer. This is a problem of this implementation, but it also allows us not to use any other buffer in between.
 */
void uart1_send_message_dma( const uint8_t* message, const uint32_t message_len )
{

    UART_TX_DMA_CH->CCR &= ~DMA_CCR_EN;

    UART_TX_DMA_CH->CMAR = (uint32_t)message;   // Set new source memory address
    UART_TX_DMA_CH->CNDTR = message_len;

    USART1->ICR = USART_ICR_TCCF; // Clear "Transmission complete" flag

    UART_TX_DMA_CH->CCR  |= DMA_CCR_MINC // Memory increment mode
                        | DMA_CCR_DIR   // Send from memory to peripheral
                        | DMA_CCR_EN;   // Enable DMA

    while ( (USART1->ISR & USART_ISR_TC) != USART_ISR_TC ){} // Check that the last message was fully sent
}

/**
 * @brief returns the length of the message received by DMA
 *
 * @update-type: #auto
 */
uint32_t uart1_get_received_message_len( void )
{
    return (UART_1_BUFFER_SIZE - UART_RX_DMA_CH->CNDTR);
}

/**
 * @brief
 *
 * @update-type: #none
 */
uint8_t* uart1_get_receive_buffer(void)
{
    return dma_buffer;
}

/**
 * @brief
 *
 * @update-type: #auto
 */
inline void uart1_reset_rx_dma( void )
{
    UART_RX_DMA_CH->CCR &= ~DMA_CCR_EN; // Disable DMA.

    UART_RX_DMA_CH->CNDTR = UART_1_BUFFER_SIZE; // Reset memory location
    UART_RX_DMA_CH->CCR  |= DMA_CCR_MINC // Memory increment mode
                        | DMA_CCR_EN;   // Enable DMA
}

/**************************************************************************************************/
/*                                                                                                */
/*                                 Static functions implementations                               */
/*                                                                                                */
/**************************************************************************************************/

/**
 * @brief
 *
 * @update-type: #manual
 */
static void setup_uart_gpio( void )
{
    // The whole function below is [#manual]

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    const uint32_t gpio_b_moder_clear_mask = ~(GPIO_MODER_Msk << GPIO_MODER_MODE6_Pos
                                            | GPIO_MODER_Msk << GPIO_MODER_MODE7_Pos);
    const uint32_t gpio_b_moder_set_mask = (GPIO_ALTERNATE_Mode << GPIO_MODER_MODE6_Pos
                                            | GPIO_ALTERNATE_Mode << GPIO_MODER_MODE7_Pos);
    GPIOB->MODER = (GPIOB->MODER & gpio_b_moder_clear_mask) | gpio_b_moder_set_mask;

    GPIOB->AFR[0] |= ALTERNATE_FUNCTION_7 << GPIO_AFRL_AFSEL6_Pos
                    | ALTERNATE_FUNCTION_7 << GPIO_AFRL_AFSEL7_Pos;
}

/**
 * @brief Sets up USART to a given baud rate
 *
 * @update-type: #manual
 */
static inline void setup_uart_peripheral( const uint32_t desired_uart_baud_rate )
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // [#manual]

    USART1->BRR = SYSTEM_CLOCK_FREQUENCY_HZ / desired_uart_baud_rate; // Baud rate for oversampling by 16
    USART1->CR1 |= USART_CR1_UE     // Start UART
//                | USART_CR1_FIFOEN  // Enable FIFOs
                | USART_CR1_RTOIE;  // Enable receiver timeout interrupt
    USART1->CR2 |= USART_CR2_RTOEN; // Enable receiver timeout;
    USART1->RTOR = 10; // number of bits before receiver timeout interrupt;

    USART1->CR3 |= USART_CR3_EIE;   // Enable errors interrupt
    NVIC_SetPriority(USART1_IRQn, 6);
    NVIC_EnableIRQ(USART1_IRQn);    // [#manual] Enable interrupt in the core

    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // enable TX and RX. TX will send IDLE frame after being enabled
}


/**
 * @brief
 *
 * @update-type: #manual
 */
static inline uint16_t setup_uart_dma( void )
{
#ifdef DEBUG_PIF_DMA_VALIDATION
    if (UART_RX_DMA_CH->CPAR != 0)
    {
        return DMA_CHANNEL_OVERLAY_ERROR_OFFSET + 11; // [#manual] Error + dma_number * 10 + channel_number
    }

    if(UART_TX_DMA_CH->CPAR != 0)
    {
        return DMA_CHANNEL_OVERLAY_ERROR_OFFSET + 12; // [#manual] Error + dma_number * 10 + channel_number
    }
#endif

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMAMUX1EN; // [#manual]

    USART1->CR3 |= USART_CR3_DMAR| USART_CR3_DMAT; // Enable DMA for RX and TX;

    UART_RX_DMAMUX_CH->CCR |= DMAMUX_CxCR_DMAREQ_USART1_RX; // [#manual]
    UART_TX_DMAMUX_CH->CCR |= DMAMUX_CxCR_DMAREQ_USART1_TX; // [#manual]

    // Set all fields for USART RX DMA
    UART_RX_DMA_CH->CPAR = (uint32_t)&(USART1->RDR);
    UART_RX_DMA_CH->CMAR = (uint32_t)dma_buffer;
    UART_RX_DMA_CH->CNDTR = UART_1_BUFFER_SIZE;     // Number of bytes to receive;
    UART_RX_DMA_CH->CCR |= DMA_CCR_MINC;
    UART_RX_DMA_CH->CCR |= DMA_CCR_EN;              // Enable RX DMA channel

    // Set only peripheral address for TX
    UART_TX_DMA_CH->CPAR = (uint32_t)&(USART1->TDR);

    return 0;
}

// EOF
