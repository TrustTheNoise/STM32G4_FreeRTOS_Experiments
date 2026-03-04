// STM32G4 specific CMSIS enchansements. Full description is in the end of the file

#pragma once

#ifndef CMSIS_ENHANCEMENT_H_
#define CMSIS_ENHANCEMENT_H_


/****************************************************************************************/
/*                                                                                      */
/*                              FLASH enhancement definitions                           */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of FLASH bank mode selection options */
#define FLASH_OPTR_DBANK_SINGLE_BANK    (0b0U)
#define FLASH_OPTR_DBANK_DUAL_BANK      (0b1U)

/*! Proper definitions of FLASH bank erase selection options */
#define FLASH_CR_BKER_BANK_1            (0b0U)
#define FLASH_CR_BKER_BANK_2            (0b1U)

/****************************************************************************************/
/*                                                                                      */
/*                              RCC enhancement definitions                             */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of ADC clock source selection options */
#define RCC_CCIPR_ADCxSEL_NO_CLOCK      (0b00U)
#define RCC_CCIPR_ADCxSEL_PLL_P         (0b01U)
#define RCC_CCIPR_ADCxSEL_SYSCLOCK      (0b10U)

/*! Proper definitions of FDCAN clock source selection options */
#define RCC_CCIPR_FDCANSEL_HSE          (0b00U)
#define RCC_CCIPR_FDCANSEL_PLLQ         (0b01U)
#define RCC_CCIPR_FDCANSEL_PCLK         (0b10U)

/*! Proper definitions of CLK48SEL clock source selection options */
#define RCC_CCIPR_CLK48SEL_HSI48        (0b00U)
#define RCC_CCIPR_CLK48SEL_PLLQ         (0b10U)

/*! Proper definitions of SAI1 clock source selection options */
#define RCC_CCIPR_SAI1SEL_SYSCLOCK       (0b00U)
#define RCC_CCIPR_SAI1SEL_PLLQ           (0b01U)
#define RCC_CCIPR_SAI1SEL_I2S_CKIN       (0b10U)
#define RCC_CCIPR_SAI1SEL_HSI16          (0b11U)



/****************************************************************************************/
/*                                                                                      */
/*                             GPIO enhancement definitions                             */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of GPIO MODE setup options */
#define GPIO_DIGITAL_IN_Mode            (0b00U)
#define GPIO_DIGITAL_OUT_Mode           (0b01U)
#define GPIO_ALTERNATE_Mode             (0b10U)
#define GPIO_ANALOG_IN_Mode             (0b11U)
#define GPIO_MODER_Msk                  (0b11U)

/*! Proper definitions of OSPEED setup options. Exact frequencies will change depending on the STM32 family*/
#define GPIO_OSPEED_VERY_LOW            (0b00U)    // Fmax - 10 MHz at 3.3V for STM32G4
#define GPIO_OSPEED_LOW                 (0b01U)    // Fmax - 50 MHz at 3.3V for STM32G4
#define GPIO_OSPEED_HIGH                (0b10U)    // Fmax - 100 MHz at 3.3V for STM32G4
#define GPIO_OSPEED_VERY_HIGH           (0b11U)    // Fmax - 180 MHz at 3.3V for STM32G4
#define GPIO_OSPEED_Msk                 (0b11U)

/*! Proper definitions of OTYPER setup options */
#define GPIO_OTYPER_PP                  (0b0U)    // push pull
#define GPIO_OTYPER_OD                  (0b1U)    // open drain

/*! Proper definitions of PUPDR setup options */
#define GPIO_PUPDR_NO_PULL              (0b00U)
#define GPIO_PUPDR_PULL_UP              (0b01U)
#define GPIO_PUPDR_PULL_DOWN            (0b10U)
#define GPIO_PUPDR_Msk                  (0b11U)

/*! Proper definitions of LCKK setup options */
#define GPIO_LCKK_NOT_LOCKED            (0b0U)
#define GPIO_LCKK_LOCKED                (0b1U)

/*! Proper definitions of alternate functions setup options */
#define ALTERNATE_FUNCTION_0            (0U)    // 0b0000
#define ALTERNATE_FUNCTION_1            (1U)    // 0b0001
#define ALTERNATE_FUNCTION_2            (2U)    // 0b0010
#define ALTERNATE_FUNCTION_3            (3U)    // 0b0011
#define ALTERNATE_FUNCTION_4            (4U)    // 0b0100
#define ALTERNATE_FUNCTION_5            (5U)    // 0b0101
#define ALTERNATE_FUNCTION_6            (6U)    // 0b0110
#define ALTERNATE_FUNCTION_7            (7U)    // 0b0111
#define ALTERNATE_FUNCTION_8            (8U)    // 0b1000
#define ALTERNATE_FUNCTION_9            (9U)    // 0b1001
#define ALTERNATE_FUNCTION_10           (10U)   // 0b1010
#define ALTERNATE_FUNCTION_11           (11U)   // 0b1011
#define ALTERNATE_FUNCTION_12           (12U)   // 0b1100
#define ALTERNATE_FUNCTION_13           (13U)   // 0b1101
#define ALTERNATE_FUNCTION_14           (14U)   // 0b1110
#define ALTERNATE_FUNCTION_15           (15U)   // 0b1111
#define ALTERNATE_FUNCTION_Msk          (15U)   // 0b1111


/****************************************************************************************/
/*                                                                                      */
/*                            DMA enhancement definitions                               */
/*                                                                                      */
/****************************************************************************************/

#define DMA_CHANNEL_1_BASE                          (0U)
#define DMA_CHANNEL_2_BASE                          (1U)
#define DMA_CHANNEL_3_BASE                          (2U)
#define DMA_CHANNEL_4_BASE                          (3U)
#define DMA_CHANNEL_5_BASE                          (4U)
#define DMA_CHANNEL_6_BASE                          (5U)
#define DMA_CHANNEL_7_BASE                          (6U)
#define DMA_CHANNEL_8_BASE                          (7U)

#define DMA_ISR_Stride                              (4U)
#define DMA_ISR_GIFx_Msk                            (0b0001)
#define DMA_ISR_TCIFx_Msk                           (0b0010)
#define DMA_ISR_HTIFx_Msk                           (0b0100)
#define DMA_ISR_TEIFx_Msk                           (0b1000)

#define DMA_IFCR_Stride                             (4U)
#define DMA_IFCR_CGIFx_Msk                          (0b0001)
#define DMA_IFCR_CTCIFx_Msk                         (0b0010)
#define DMA_IFCR_CHTIFx_Msk                         (0b0100)
#define DMA_IFCR_CTEIFx_Msk                         (0b1000)

#define DMA_CCR_MSIZE_8_BITS                       (0b00U)
#define DMA_CCR_MSIZE_16_BITS                      (0b01U)
#define DMA_CCR_MSIZE_32_BITS                      (0b10U)

#define DMA_CCR_PSIZE_8_BITS                       (0b00U)
#define DMA_CCR_PSIZE_16_BITS                      (0b01U)
#define DMA_CCR_PSIZE_32_BITS                      (0b10U)


/****************************************************************************************/
/*                                                                                      */
/*                            DMAMUX enhancement definitions                            */
/*                                                                                      */
/****************************************************************************************/

// @todo Finish all these definitions
// @todo Document new idea that postfix _s = setting, so it must not use "<< xx_Pos". And _o = option, so it must use "<< xx_Pos". (WTF did I just read? @todo fix this idea, complitely unclear to me)

#define DMAMUX_CxCR_DMAREQ_DMAMUX_Req_G0            (1U)    // 0b0000001
#define DMAMUX_CxCR_DMAREQ_DMAMUX_Req_G1            (2U)    // 0b0000010
#define DMAMUX_CxCR_DMAREQ_DMAMUX_Req_G2            (3U)    // 0b0000011
#define DMAMUX_CxCR_DMAREQ_DMAMUX_Req_G3            (4U)    // 0b0000100
#define DMAMUX_CxCR_DMAREQ_ADC1                     (5U)    // 0b0000101
#define DMAMUX_CxCR_DMAREQ_DAC1_CH1                 (6U)    // 0b0000110
#define DMAMUX_CxCR_DMAREQ_DAC1_CH2                 (7U)    // 0b0000111
#define DMAMUX_CxCR_DMAREQ_TIM6_UP                  (8U)    // 0b0001000
#define DMAMUX_CxCR_DMAREQ_TIM7_UP                  (9U)    // 0b0001001
#define DMAMUX_CxCR_DMAREQ_SPI1_RX                  (10U)   // 0b0001010
#define DMAMUX_CxCR_DMAREQ_SPI1_TX                  (11U)   // 0b0001011
#define DMAMUX_CxCR_DMAREQ_SPI2_RX                  (12U)   // 0b0001100
#define DMAMUX_CxCR_DMAREQ_SPI2_TX                  (13U)   // 0b0001101
#define DMAMUX_CxCR_DMAREQ_SPI3_RX                  (14U)   // 0b0001110
#define DMAMUX_CxCR_DMAREQ_SPI3_TX                  (15U)   // 0b0001111
#define DMAMUX_CxCR_DMAREQ_I2C1_RX                  (16U)   // 0b0010000
#define DMAMUX_CxCR_DMAREQ_I2C1_TX                  (17U)   // 0b0010001
#define DMAMUX_CxCR_DMAREQ_I2C2_RX                  (18U)   // 0b0010010
#define DMAMUX_CxCR_DMAREQ_I2C2_TX                  (19U)   // 0b0010011
#define DMAMUX_CxCR_DMAREQ_I2C3_RX                  (20U)   // 0b0010100
#define DMAMUX_CxCR_DMAREQ_I2C3_TX                  (21U)   // 0b0010101
#define DMAMUX_CxCR_DMAREQ_I2C4_RX                  (22U)   // 0b0010110
#define DMAMUX_CxCR_DMAREQ_I2C4_TX                  (23U)   // 0b0010111
#define DMAMUX_CxCR_DMAREQ_USART1_RX                (24U)   // 0b0011000
#define DMAMUX_CxCR_DMAREQ_USART1_TX                (25U)   // 0b0011001
#define DMAMUX_CxCR_DMAREQ_USART2_RX                (26U)   // 0b0011010
#define DMAMUX_CxCR_DMAREQ_USART2_TX                (27U)   // 0b0011011
#define DMAMUX_CxCR_DMAREQ_USART3_RX                (28U)   // 0b0011100
#define DMAMUX_CxCR_DMAREQ_USART3_TX                (29U)   // 0b0011101
#define DMAMUX_CxCR_DMAREQ_UART4_RX                 (30U)   // 0b0011110
#define DMAMUX_CxCR_DMAREQ_UART4_TX                 (31U)   // 0b0011111
#define DMAMUX_CxCR_DMAREQ_UART5_RX                 (32U)   // 0b0100000
#define DMAMUX_CxCR_DMAREQ_UART5_TX                 (33U)   // 0b0100001
#define DMAMUX_CxCR_DMAREQ_LPUART1_RX               (34U)   // 0b0100010
#define DMAMUX_CxCR_DMAREQ_LPUART1_TX               (35U)   // 0b0100011
#define DMAMUX_CxCR_DMAREQ_ADC2                     (36U)   // 0b0100100
#define DMAMUX_CxCR_DMAREQ_ADC3                     (37U)   // 0b0100101
#define DMAMUX_CxCR_DMAREQ_ADC4                     (38U)   // 0b0100110
#define DMAMUX_CxCR_DMAREQ_ADC5                     (39U)   // 0b0100111
#define DMAMUX_CxCR_DMAREQ_QUADSPI                  (40U)   // 0b0101000
#define DMAMUX_CxCR_DMAREQ_DAC_CH1                  (41U)   // 0b0101001
#define DMAMUX_CxCR_DMAREQ_TIM1_CH1                 (42U)   // 0b0101010
#define DMAMUX_CxCR_DMAREQ_TIM1_CH2                 (43U)   // 0b0101011
#define DMAMUX_CxCR_DMAREQ_TIM1_CH3                 (44U)   // 0b0101100
#define DMAMUX_CxCR_DMAREQ_TIM1_CH4                 (45U)   // 0b0101101
#define DMAMUX_CxCR_DMAREQ_TIM1_UP                  (46U)   // 0b0101110
#define DMAMUX_CxCR_DMAREQ_TIM1_TRIG                (47U)   // 0b0101111
#define DMAMUX_CxCR_DMAREQ_TIM1_COM                 (48U)   // 0b0110000
#define DMAMUX_CxCR_DMAREQ_TIM8_CH1                 (49U)   // 0b0110001
#define DMAMUX_CxCR_DMAREQ_TIM8_CH2                 (50U)   // 0b0110010
#define DMAMUX_CxCR_DMAREQ_TIM8_CH3                 (51U)   // 0b0110011
#define DMAMUX_CxCR_DMAREQ_TIM8_CH4                 (52U)   // 0b0110100
#define DMAMUX_CxCR_DMAREQ_TIM8_UP                  (53U)   // 0b0110101
#define DMAMUX_CxCR_DMAREQ_TIM8_TRIG                (54U)   // 0b0110110
#define DMAMUX_CxCR_DMAREQ_TIM8_COM                 (55U)   // 0b0110111
#define DMAMUX_CxCR_DMAREQ_TIM2_CH1                 (56U)   // 0b0111000
#define DMAMUX_CxCR_DMAREQ_TIM2_CH2                 (57U)   // 0b0111001
#define DMAMUX_CxCR_DMAREQ_TIM2_CH3                 (58U)   // 0b0111010
#define DMAMUX_CxCR_DMAREQ_TIM2_CH4                 (59U)   // 0b0111011
#define DMAMUX_CxCR_DMAREQ_TIM2_UP                  (60U)   // 0b0111100
#define DMAMUX_CxCR_DMAREQ_TIM3_CH1                 (61U)   // 0b0111101
#define DMAMUX_CxCR_DMAREQ_TIM3_CH2                 (62U)   // 0b0111110
#define DMAMUX_CxCR_DMAREQ_TIM3_CH3                 (63U)   // 0b0111111
#define DMAMUX_CxCR_DMAREQ_TIM3_CH4                 (64U)   // 0b1000000
#define DMAMUX_CxCR_DMAREQ_TIM3_UP                  (65U)   // 0b1000001
#define DMAMUX_CxCR_DMAREQ_TIM3_TRIG                (66U)   // 0b1000010
#define DMAMUX_CxCR_DMAREQ_TIM4_CH1                 (67U)   // 0b1000011
#define DMAMUX_CxCR_DMAREQ_TIM4_CH2                 (68U)   // 0b1000100
#define DMAMUX_CxCR_DMAREQ_TIM4_CH3                 (69U)   // 0b1000101
#define DMAMUX_CxCR_DMAREQ_TIM4_CH4                 (70U)   // 0b1000110
#define DMAMUX_CxCR_DMAREQ_TIM4_UP                  (71U)   // 0b1000111
#define DMAMUX_CxCR_DMAREQ_TIM5_CH1                 (72U)   // 0b1001000
#define DMAMUX_CxCR_DMAREQ_TIM5_CH2                 (73U)   // 0b1001001
#define DMAMUX_CxCR_DMAREQ_TIM5_CH3                 (74U)   // 0b1001010
#define DMAMUX_CxCR_DMAREQ_TIM5_CH4                 (75U)   // 0b1001011
#define DMAMUX_CxCR_DMAREQ_TIM5_UP                  (76U)   // 0b1001100
#define DMAMUX_CxCR_DMAREQ_TIM5_TRIG                (77U)   // 0b1001101
#define DMAMUX_CxCR_DMAREQ_TIM15_CH1                (78U)   // 0b1001110
#define DMAMUX_CxCR_DMAREQ_TIM15_UP                 (79U)   // 0b1001111
#define DMAMUX_CxCR_DMAREQ_TIM15_TRIG               (80U)   // 0b1010000
#define DMAMUX_CxCR_DMAREQ_TIM15_COM                (81U)   // 0b1010001
#define DMAMUX_CxCR_DMAREQ_TIM16_CH1                (82U)   // 0b1010010
#define DMAMUX_CxCR_DMAREQ_TIM16_UP                 (83U)   // 0b1010011
#define DMAMUX_CxCR_DMAREQ_TIM17_CH1                (84U)   // 0b1010100
#define DMAMUX_CxCR_DMAREQ_TIM17_UP                 (85U)   // 0b1010101
#define DMAMUX_CxCR_DMAREQ_TIM20_CH1                (86U)   // 0b1010110
#define DMAMUX_CxCR_DMAREQ_TIM20_CH2                (87U)   // 0b1010111
#define DMAMUX_CxCR_DMAREQ_TIM20_CH3                (88U)   // 0b1011000
#define DMAMUX_CxCR_DMAREQ_TIM20_CH4                (89U)   // 0b1011001
#define DMAMUX_CxCR_DMAREQ_TIM20_UP                 (90U)   // 0b1011010
#define DMAMUX_CxCR_DMAREQ_AES_IN                   (91U)   // 0b1011011
#define DMAMUX_CxCR_DMAREQ_AES_OUT                  (92U)   // 0b1011100
#define DMAMUX_CxCR_DMAREQ_TIM20_TRIG               (93U)   // 0b1011101
#define DMAMUX_CxCR_DMAREQ_TIM20_COM                (94U)   // 0b1011110
#define DMAMUX_CxCR_DMAREQ_HTRIM_MASTER             (95U)   // 0b1011111
#define DMAMUX_CxCR_DMAREQ_HTRIM_TIMA               (96U)   // 0b1100000
#define DMAMUX_CxCR_DMAREQ_HTRIM_TIMB               (97U)   // 0b1100001
#define DMAMUX_CxCR_DMAREQ_HTRIM_TIMC               (98U)   // 0b1100010
#define DMAMUX_CxCR_DMAREQ_HTRIM_TIMD               (99U)   // 0b1100011
#define DMAMUX_CxCR_DMAREQ_HTRIM_TIME               (100U)  // 0b1100100
#define DMAMUX_CxCR_DMAREQ_HTRIM_TIMF               (101U)  // 0b1100101
#define DMAMUX_CxCR_DMAREQ_DAC3_CH1                 (102U)  // 0b1100110
#define DMAMUX_CxCR_DMAREQ_DAC3_CH2                 (103U)  // 0b1100111
#define DMAMUX_CxCR_DMAREQ_DAC4_CH1                 (104U)  // 0b1101000
#define DMAMUX_CxCR_DMAREQ_DAC4_CH2                 (105U)  // 0b1101001
#define DMAMUX_CxCR_DMAREQ_SPI4_RX                  (106U)  // 0b1101010
#define DMAMUX_CxCR_DMAREQ_SPI4_TX                  (107U)  // 0b1101011
#define DMAMUX_CxCR_DMAREQ_SAI1_A                   (108U)  // 0b1101100
#define DMAMUX_CxCR_DMAREQ_SAI1_B                   (109U)  // 0b1101101
#define DMAMUX_CxCR_DMAREQ_FMAC_Read                (110U)  // 0b1101110
#define DMAMUX_CxCR_DMAREQ_FMAC_Write               (111U)  // 0b1101111
#define DMAMUX_CxCR_DMAREQ_Cordic_Read              (112U)  // 0b1110000
#define DMAMUX_CxCR_DMAREQ_Cordic_Write             (113U)  // 0b1110001
#define DMAMUX_CxCR_DMAREQ_UCPD1_RX                 (114U)  // 0b1110010
#define DMAMUX_CxCR_DMAREQ_UCPD1_TX                 (115U)  // 0b1110011

/****************************************************************************************/
/*                                                                                      */
/*                              TIM enhancement definitions                             */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of TIM CCMR output compare mode selection options */
#define TIM_CCMR_OCxM_Frozen                            (0b0U << 12 | 0b000U)
#define TIM_CCMR_OCxM_Active_on_match                   (0b0U << 12 | 0b001U)
#define TIM_CCMR_OCxM_Inactive_on_match                 (0b0U << 12 | 0b010U)
#define TIM_CCMR_OCxM_Toggle                            (0b0U << 12 | 0b011U)
#define TIM_CCMR_OCxM_Force_inactive                    (0b0U << 12 | 0b100U)
#define TIM_CCMR_OCxM_Force_active                      (0b0U << 12 | 0b101U)
#define TIM_CCMR_OCxM_PWM1                              (0b0U << 12 | 0b110U)
#define TIM_CCMR_OCxM_PWM2                              (0b0U << 12 | 0b111U)
#define TIM_CCMR_OCxM_Retrigerrable_OPM1                (0b1U << 12 | 0b000U)
#define TIM_CCMR_OCxM_Retrigerrable_OPM2                (0b1U << 12 | 0b001U)
#define TIM_CCMR_OCxM_Combined_PWM1                     (0b1U << 12 | 0b100U)
#define TIM_CCMR_OCxM_Combined_PWM2                     (0b1U << 12 | 0b101U)
#define TIM_CCMR_OCxM_Asymmetric_PWM1                   (0b1U << 12 | 0b110U)
#define TIM_CCMR_OCxM_Asymmetric_PWM2                   (0b1U << 12 | 0b111U)
// The reason for such a division in the last value is that in OCxM,
// the most significant bit is separated from the others.
// The reference manual states that this is done for compatibility.
// Therefore, this separation in the define is necessary so that you
// can write *option* << TIM_CCMR_OC2M_Pos.

/*! Proper definitions of TIM CR1 сenter-aligned mode selection*/
#define TIM_CR1_CMS_EDGE_ALIGN                          (0b00U)
#define TIM_CR1_CMS_CENTER_ALIGN_DOWN                   (0b01U)
#define TIM_CR1_CMS_CENTER_ALIGN_UP                     (0b10U)
#define TIM_CR1_CMS_CENTER_ALIGN_UP_DOWN                (0b11U)

/*! Proper definitions of TIM CR1 clock division selection*/
#define TIM_CR1_CKD_DIVISION_1                          (0b00U)
#define TIM_CR1_CKD_DIVISION_2                          (0b01U)
#define TIM_CR1_CKD_DIVISION_4                          (0b10U)

/****************************************************************************************/
/*                                                                                      */
/*                              TIM2/3/4 enhancement definitions                        */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of TIM CR2 master mode selection options */
#define TIM234_CR2_MMS_Reset                    (0b0 << 21 | 0b000U)
#define TIM234_CR2_MMS_Enable                   (0b0 << 21 | 0b001U)
#define TIM234_CR2_MMS_Update                   (0b0 << 21 | 0b010U)
#define TIM234_CR2_MMS_Compare_Pulse            (0b0 << 21 | 0b011U)
#define TIM234_CR2_MMS_Compare_OC1              (0b0 << 21 | 0b100U)
#define TIM234_CR2_MMS_Compare_OC2              (0b0 << 21 | 0b101U)
#define TIM234_CR2_MMS_Compare_OC3              (0b0 << 21 | 0b110U)
#define TIM234_CR2_MMS_Compare_OC4              (0b0 << 21 | 0b111U)
#define TIM234_CR2_MMS_Encoder_Clock            (0b1 << 21 | 0b000U)

/****************************************************************************************/
/*                                                                                      */
/*                              TIM1/8/20 enhancement definitions                        */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of TIM CR2 master mode selection options */
#define TIM1820_CR2_MMS_Reset                    (0b0 << 21 | 0b000U)
#define TIM1820_CR2_MMS_Enable                   (0b0 << 21 | 0b001U)
#define TIM1820_CR2_MMS_Update                   (0b0 << 21 | 0b010U)
#define TIM1820_CR2_MMS_Compare_Pulse            (0b0 << 21 | 0b011U)
#define TIM1820_CR2_MMS_Compare_OC1              (0b0 << 21 | 0b100U)
#define TIM1820_CR2_MMS_Compare_OC2              (0b0 << 21 | 0b101U)
#define TIM1820_CR2_MMS_Compare_OC3              (0b0 << 21 | 0b110U)
#define TIM1820_CR2_MMS_Compare_OC4              (0b0 << 21 | 0b111U)
#define TIM1820_CR2_MMS_Encoder_Clock            (0b1 << 21 | 0b000U)

/*! Proper definitions of TIM CR2 master mode selection 2 options */
#define TIM1820_CR2_MMS2_Reset                              (0b0000U)
#define TIM1820_CR2_MMS2_Enable                             (0b0001U)
#define TIM1820_CR2_MMS2_Update                             (0b0010U)
#define TIM1820_CR2_MMS2_Compare_Pulse                      (0b0011U)
#define TIM1820_CR2_MMS2_Compare_OC1                        (0b0100U)
#define TIM1820_CR2_MMS2_Compare_OC2                        (0b0101U)
#define TIM1820_CR2_MMS2_Compare_OC3                        (0b0110U)
#define TIM1820_CR2_MMS2_Compare_OC4                        (0b0111U)
#define TIM1820_CR2_MMS2_Compare_OC5                        (0b1000U)
#define TIM1820_CR2_MMS2_Compare_OC6                        (0b1001U)
#define TIM1820_CR2_MMS2_Compare_Pulse_OC4_RF               (0b1010U)
#define TIM1820_CR2_MMS2_Compare_Pulse_OC6_RF               (0b1011U)
#define TIM1820_CR2_MMS2_Compare_Pulse_OC46_R               (0b1100U)
#define TIM1820_CR2_MMS2_Compare_Pulse_OC4_R_OC6_F          (0b1101U)
#define TIM1820_CR2_MMS2_Compare_Pulse_OC56_R               (0b1110U)
#define TIM1820_CR2_MMS2_Compare_Pulse_OC56_F               (0b1111U)

/****************************************************************************************/
/*                                                                                      */
/*                              SPI enhancement definitions                             */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of SPI CS2 data size options */
#define SPI_CS2_DS_4_BITS               (0b0011U)
#define SPI_CS2_DS_5_BITS               (0b0100U)
#define SPI_CS2_DS_6_BITS               (0b0101U)
#define SPI_CS2_DS_7_BITS               (0b0110U)
#define SPI_CS2_DS_8_BITS               (0b0111U)
#define SPI_CS2_DS_9_BITS               (0b1000U)
#define SPI_CS2_DS_10_BITS              (0b1001U)
#define SPI_CS2_DS_11_BITS              (0b1010U)
#define SPI_CS2_DS_12_BITS              (0b1011U)
#define SPI_CS2_DS_13_BITS              (0b1100U)
#define SPI_CS2_DS_14_BITS              (0b1101U)
#define SPI_CS2_DS_15_BITS              (0b1110U)
#define SPI_CS2_DS_16_BITS              (0b1111U)


/****************************************************************************************/
/*                                                                                      */
/*                              SAI enhancement definitions                             */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of SAI xCS1 data size options */
#define SAI_xCR1_DS_8_Bits              (0b010U)
#define SAI_xCR1_DS_10_Bits             (0b011U)
#define SAI_xCR1_DS_16_Bits             (0b100U)
#define SAI_xCR1_DS_20_Bits             (0b101U)
#define SAI_xCR1_DS_24_Bits             (0b110U)
#define SAI_xCR1_DS_32_Bits             (0b111U)

/*! Proper definitions of SAI xCS1 mode options */
#define SAI_xCR1_MODE_Master_TX         (0b00)
#define SAI_xCR1_MODE_Master_RX         (0b01)
#define SAI_xCR1_MODE_Slave_TX          (0b10)
#define SAI_xCR1_MODE_Slave_RX          (0b11)

/*! Proper definitions of SAI xCS2 FIFO threshold options */
#define SAI_xCR2_FTH_Empty              (0b000)
#define SAI_xCR2_FTH_Quarter            (0b001)
#define SAI_xCR2_FTH_Half               (0b010)
#define SAI_xCR2_FTH_Three_Quarter      (0b011)
#define SAI_xCR2_FTH_Full               (0b100)

/*! Proper definitions of SAI xSLOTR number of slots options */
#define SAI_xSLOTR_NBSLOT_1_Slots        (0b0000)
#define SAI_xSLOTR_NBSLOT_2_Slots        (0b0001)
#define SAI_xSLOTR_NBSLOT_3_Slots        (0b0010)
#define SAI_xSLOTR_NBSLOT_4_Slots        (0b0011)
#define SAI_xSLOTR_NBSLOT_5_Slots        (0b0100)
#define SAI_xSLOTR_NBSLOT_6_Slots        (0b0101)
#define SAI_xSLOTR_NBSLOT_7_Slots        (0b0110)
#define SAI_xSLOTR_NBSLOT_8_Slots        (0b0111)
#define SAI_xSLOTR_NBSLOT_9_Slots        (0b1000)
#define SAI_xSLOTR_NBSLOT_10_Slots       (0b1001)
#define SAI_xSLOTR_NBSLOT_11_Slots       (0b1010)
#define SAI_xSLOTR_NBSLOT_12_Slots       (0b1011)
#define SAI_xSLOTR_NBSLOT_13_Slots       (0b1100)
#define SAI_xSLOTR_NBSLOT_14_Slots       (0b1101)
#define SAI_xSLOTR_NBSLOT_15_Slots       (0b1110)
#define SAI_xSLOTR_NBSLOT_16_Slots       (0b1111)

/*! Proper definitions of SAI xSLOTR slot size options */
#define SAI_xSLOTR_SLOTSZ_Data_Size     (0b00)
#define SAI_xSLOTR_SLOTSZ_16_Bits       (0b01)
#define SAI_xSLOTR_SLOTSZ_32_Bits       (0b10)

/*! Proper defition of SAI xSLOTR slot enable options */
#define SAI_xSLOTR_SLOTEN_Slot_1_EN     (1U << 0)
#define SAI_xSLOTR_SLOTEN_Slot_2_EN     (1U << 1)
#define SAI_xSLOTR_SLOTEN_Slot_3_EN     (1U << 2)
#define SAI_xSLOTR_SLOTEN_Slot_4_EN     (1U << 3)
#define SAI_xSLOTR_SLOTEN_Slot_5_EN     (1U << 4)
#define SAI_xSLOTR_SLOTEN_Slot_6_EN     (1U << 5)
#define SAI_xSLOTR_SLOTEN_Slot_7_EN     (1U << 6)
#define SAI_xSLOTR_SLOTEN_Slot_8_EN     (1U << 7)
#define SAI_xSLOTR_SLOTEN_Slot_9_EN     (1U << 8)
#define SAI_xSLOTR_SLOTEN_Slot_10_EN    (1U << 9)
#define SAI_xSLOTR_SLOTEN_Slot_11_EN    (1U << 10)
#define SAI_xSLOTR_SLOTEN_Slot_12_EN    (1U << 11)
#define SAI_xSLOTR_SLOTEN_Slot_13_EN    (1U << 12)
#define SAI_xSLOTR_SLOTEN_Slot_14_EN    (1U << 13)
#define SAI_xSLOTR_SLOTEN_Slot_15_EN    (1U << 14)
#define SAI_xSLOTR_SLOTEN_Slot_16_EN    (1U << 15)


/****************************************************************************************/
/*                                                                                      */
/*                              ADC enhancement definitions                             */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of ADC CFGR resolution selection options */
#define ADC_CFGR_RES_12_BIT             (0b00U)
#define ADC_CFGR_RES_10_BIT             (0b01U)
#define ADC_CFGR_RES_8_BIT              (0b10U)
#define ADC_CFGR_RES_6_BIT              (0b11U)

/*! Proper definitions of ADC CFGR EXTSEL selecting an external trigger for regular channels settings */
#define ADC_CFGR_EXTSEL_ADC12_TIM1_CC1                      (0U)    // 0b00000
#define ADC_CFGR_EXTSEL_ADC12_TIM1_CC2                      (1U)    // 0b00001
#define ADC_CFGR_EXTSEL_ADC12_TIM1_CC3                      (2U)    // 0b00010
#define ADC_CFGR_EXTSEL_ADC12_TIM2_CC2                      (3U)    // 0b00011
#define ADC_CFGR_EXTSEL_ADC12_TIM3_TRGO                     (4U)    // 0b00100
#define ADC_CFGR_EXTSEL_ADC12_TIM4_CC4                      (5U)    // 0b00101
#define ADC_CFGR_EXTSEL_ADC12_EXTI11                        (6U)    // 0b00110
#define ADC_CFGR_EXTSEL_ADC12_TIM8_TRGO                     (7U)    // 0b00111
#define ADC_CFGR_EXTSEL_ADC12_TIM8_TRGO2                    (8U)    // 0b01000
#define ADC_CFGR_EXTSEL_ADC12_TIM1_TRGO                     (9U)    // 0b01001
#define ADC_CFGR_EXTSEL_ADC12_TIM1_TRGO2                    (10U)   // 0b01010
#define ADC_CFGR_EXTSEL_ADC12_TIM2_TRGO                     (11U)   // 0b01011
#define ADC_CFGR_EXTSEL_ADC12_TIM4_TRGO                     (12U)   // 0b01100
#define ADC_CFGR_EXTSEL_ADC12_TIM5_TRGO                     (13U)   // 0b01101
#define ADC_CFGR_EXTSEL_ADC12_TIM15_TRGO                    (14U)   // 0b01110
#define ADC_CFGR_EXTSEL_ADC12_TIM3_CC4                      (15U)   // 0b01111
#define ADC_CFGR_EXTSEL_ADC12_TIM20_TRGO                    (16U)   // 0b10000
#define ADC_CFGR_EXTSEL_ADC12_TIM20_TRGO2                   (17U)   // 0b10001
#define ADC_CFGR_EXTSEL_ADC12_TIM20_CC1                     (18U)   // 0b10010
#define ADC_CFGR_EXTSEL_ADC12_TIM20_CC2                     (19U)   // 0b10011
#define ADC_CFGR_EXTSEL_ADC12_TIM20_CC3                     (20U)   // 0b10100
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG1                      (21U)   // 0b10101
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG3                      (22U)   // 0b10110
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG5                      (23U)   // 0b10111
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG6                      (24U)   // 0b11000
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG7                      (25U)   // 0b11001
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG8                      (26U)   // 0b11010
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG9                      (27U)   // 0b11011
#define ADC_CFGR_EXTSEL_HRTIM_ADC_TRG10                     (28U)   // 0b11100
#define ADC_CFGR_EXTSEL_LPTIMOUT                            (29U)   // 0b11101
#define ADC_CFGR_EXTSEL_TIM7_TRGO                           (30U)   // 0b11110

#define ADC_CFGR_EXTSEL_ADC345_TIM3_CC1                     (0U)    // 0b00000
#define ADC_CFGR_EXTSEL_ADC345_TIM2_CC3                     (1U)    // 0b00001
#define ADC_CFGR_EXTSEL_ADC345_TIM1_CC3                     (2U)    // 0b00010
#define ADC_CFGR_EXTSEL_ADC345_TIM8_CC1                     (3U)    // 0b00011
#define ADC_CFGR_EXTSEL_ADC345_TIM3_TRGO                    (4U)    // 0b00100
#define ADC_CFGR_EXTSEL_ADC345_EXTI2                        (5U)    // 0b00101
#define ADC_CFGR_EXTSEL_ADC345_TIM4_CC1                     (6U)    // 0b00110
#define ADC_CFGR_EXTSEL_ADC345_TIM8_TRGO                    (7U)    // 0b00111
#define ADC_CFGR_EXTSEL_ADC345_TIM8_TRGO2                   (8U)    // 0b01000
#define ADC_CFGR_EXTSEL_ADC345_TIM1_TRGO                    (9U)    // 0b01001
#define ADC_CFGR_EXTSEL_ADC345_TIM1_TRGO2                   (10U)   // 0b01010
#define ADC_CFGR_EXTSEL_ADC345_TIM2_TRGO                    (11U)   // 0b01011
#define ADC_CFGR_EXTSEL_ADC345_TIM4_TRGO                    (12U)   // 0b01100
#define ADC_CFGR_EXTSEL_ADC345_TIM6_TRGO                    (13U)   // 0b01101
#define ADC_CFGR_EXTSEL_ADC345_TIM15_TRGO                   (14U)   // 0b01110
#define ADC_CFGR_EXTSEL_ADC345_TIM2_CC1                     (15U)   // 0b01111
#define ADC_CFGR_EXTSEL_ADC345_TIM20_TRGO                   (16U)   // 0b10000
#define ADC_CFGR_EXTSEL_ADC345_TIM20_TRGO2                  (17U)   // 0b10001
#define ADC_CFGR_EXTSEL_ADC345_TIM20_CC1                    (18U)   // 0b10010
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG2               (19U)   // 0b10011
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG4               (20U)   // 0b10100
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG1               (21U)   // 0b10101
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG3               (22U)   // 0b10110
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG5               (23U)   // 0b10111
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG6               (24U)   // 0b11000
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG7               (25U)   // 0b11001
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG8               (26U)   // 0b11010
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG9               (27U)   // 0b11011
#define ADC_CFGR_EXTSEL_ADC345_HRTIM_ADC_TRG10              (28U)   // 0b11100
#define ADC_CFGR_EXTSEL_ADC345_LPTIMOUT                     (29U)   // 0b11101
#define ADC_CFGR_EXTSEL_ADC345_TIM7_TRGO                    (30U)   // 0b11110


/*! Proper definitions of ADC SMPR sampling time selection options. CC = clock cycles */
#define ADC_SMPR_2p5_CC                 (0b000U)
#define ADC_SMPR_6p5_CC                 (0b001U)
#define ADC_SMPR_12p5_CC                (0b010U)
#define ADC_SMPR_24p5_CC                (0b011U)
#define ADC_SMPR_47p5_CC                (0b100U)
#define ADC_SMPR_92p5_CC                (0b101U)
#define ADC_SMPR_247p5_CC               (0b110U)
#define ADC_SMPR_640p5_CC               (0b111U)

/*! Proper definitions of ADC SQR L number of conversions selection options */
#define ADC_SQR_L_1_CONVERSION          (0U)    // 0b0000
#define ADC_SQR_L_2_CONVERSIONS         (1U)    // 0b0001
#define ADC_SQR_L_3_CONVERSIONS         (2U)    // 0b0010
#define ADC_SQR_L_4_CONVERSIONS         (3U)    // 0b0011
#define ADC_SQR_L_5_CONVERSIONS         (4U)    // 0b0100
#define ADC_SQR_L_6_CONVERSIONS         (5U)    // 0b0101
#define ADC_SQR_L_7_CONVERSIONS         (6U)    // 0b0110
#define ADC_SQR_L_8_CONVERSIONS         (7U)    // 0b0111
#define ADC_SQR_L_9_CONVERSIONS         (8U)    // 0b1000
#define ADC_SQR_L_10_CONVERSIONS        (9U)    // 0b1001
#define ADC_SQR_L_11_CONVERSIONS        (10U)   // 0b1010
#define ADC_SQR_L_12_CONVERSIONS        (11U)   // 0b1011
#define ADC_SQR_L_13_CONVERSIONS        (12U)   // 0b1100
#define ADC_SQR_L_14_CONVERSIONS        (13U)   // 0b1101
#define ADC_SQR_L_15_CONVERSIONS        (14U)   // 0b1110
#define ADC_SQR_L_16_CONVERSIONS        (15U)   // 0b1111


/*! Proper definitions of ADC SQR JL number of conversions selection options */
#define ADC_JSQR_JL_1_CONVERSION        (0b00U)
#define ADC_JSQR_JL_2_CONVERSIONS       (0b01U)
#define ADC_JSQR_JL_3_CONVERSIONS       (0b10U)
#define ADC_JSQR_JL_4_CONVERSIONS       (0b11U)

/*! Proper definitions of ADC SQR JEXTEN external trigger polarity selection options */
#define ADC_JSQR_JEXTEN_SW_TRIG          (0b00U)
#define ADC_JSQR_JEXTEN_HW_TRIG_R        (0b01U)
#define ADC_JSQR_JEXTEN_HW_TRIG_F        (0b10U)
#define ADC_JSQR_JEXTEN_HW_TRIG_RF       (0b11U)

/*! Proper definitions of ADC JSQR JEXTSEL external trigger for injected channels settings*/
#define ADC_JSQR_JEXTSEL_ADC12_TIM1_TRGO                    (0U)    // 0b00000
#define ADC_JSQR_JEXTSEL_ADC12_TIM1_CC4                     (1U)    // 0b00001
#define ADC_JSQR_JEXTSEL_ADC12_TIM2_TRGO                    (2U)    // 0b00010
#define ADC_JSQR_JEXTSEL_ADC12_TIM2_CC1                     (3U)    // 0b00011
#define ADC_JSQR_JEXTSEL_ADC12_TIM3_CC4                     (4U)    // 0b00100
#define ADC_JSQR_JEXTSEL_ADC12_TIM4_TRGO                    (5U)    // 0b00101
#define ADC_JSQR_JEXTSEL_EXTI15                             (6U)    // 0b00110
#define ADC_JSQR_JEXTSEL_ADC12_TIM8_CC4                     (7U)    // 0b00111
#define ADC_JSQR_JEXTSEL_ADC12_TIM1_TRGO2                   (8U)    // 0b01000
#define ADC_JSQR_JEXTSEL_ADC12_TIM8_TRGO                    (9U)    // 0b01001
#define ADC_JSQR_JEXTSEL_ADC12_TIM8_TRGO2                   (10U)   // 0b01010
#define ADC_JSQR_JEXTSEL_ADC12_TIM3_CC3                     (11U)   // 0b01011
#define ADC_JSQR_JEXTSEL_ADC12_TIM3_TRGO                    (12U)   // 0b01100
#define ADC_JSQR_JEXTSEL_ADC12_TIM3_CC1                     (13U)   // 0b01101
#define ADC_JSQR_JEXTSEL_ADC12_TIM6_TRGO                    (14U)   // 0b01110
#define ADC_JSQR_JEXTSEL_ADC12_TIM15_TRGO                   (15U)   // 0b01111
#define ADC_JSQR_JEXTSEL_ADC12_TIM20_TRGO                   (16U)   // 0b10000
#define ADC_JSQR_JEXTSEL_ADC12_TIM20_TRGO2                  (17U)   // 0b10001
#define ADC_JSQR_JEXTSEL_ADC12_TIM20_CC4                    (18U)   // 0b10010
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG2               (19U)   // 0b10011
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG4               (20U)   // 0b10100
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG5               (21U)   // 0b10101
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG6               (22U)   // 0b10110
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG7               (23U)   // 0b10111
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG8               (24U)   // 0b11000
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG9               (25U)   // 0b11001
#define ADC_JSQR_JEXTSEL_ADC12_HRTIM_ADC_TRG10              (26U)   // 0b11010
#define ADC_JSQR_JEXTSEL_ADC12_TIM16_CC1                    (27U)   // 0b11011
#define ADC_JSQR_JEXTSEL_ADC12_LPTIMOUT                     (29U)   // 0b11101
#define ADC_JSQR_JEXTSEL_ADC12_TIM7_TRGO                    (30U)   // 0b11110

#define ADC_JSQR_JEXTSEL_ADC345_TIM1_TRGO                   (0U)    // 0b00000
#define ADC_JSQR_JEXTSEL_ADC345_TIM1_CC4                    (1U)    // 0b00001
#define ADC_JSQR_JEXTSEL_ADC345_TIM2_TRGO                   (2U)    // 0b00010
#define ADC_JSQR_JEXTSEL_ADC345_TIM8_CC2                    (3U)    // 0b00011
#define ADC_JSQR_JEXTSEL_ADC345_TIM4_CC3                    (4U)    // 0b00100
#define ADC_JSQR_JEXTSEL_ADC345_TIM4_TRGO                   (5U)    // 0b00101
#define ADC_JSQR_JEXTSEL_ADC345_TIM4_CC4                    (6U)    // 0b00110
#define ADC_JSQR_JEXTSEL_ADC345_TIM8_CC4                    (7U)    // 0b00111
#define ADC_JSQR_JEXTSEL_ADC345_TIM1_TRGO2                  (8U)    // 0b01000
#define ADC_JSQR_JEXTSEL_ADC345_TIM8_TRGO                   (9U)    // 0b01001
#define ADC_JSQR_JEXTSEL_ADC345_TIM8_TRGO2                  (10U)   // 0b01010
#define ADC_JSQR_JEXTSEL_ADC345_TIM1_CC3                    (11U)   // 0b01011
#define ADC_JSQR_JEXTSEL_ADC345_TIM3_TRGO                   (12U)   // 0b01100
#define ADC_JSQR_JEXTSEL_ADC345_EXTI3                       (13U)   // 0b01101
#define ADC_JSQR_JEXTSEL_ADC345_TIM6_TRGO                   (14U)   // 0b01110
#define ADC_JSQR_JEXTSEL_ADC345_TIM15_TRGO                  (15U)   // 0b01111
#define ADC_JSQR_JEXTSEL_ADC345_TIM20_TRGO                  (16U)   // 0b10000
#define ADC_JSQR_JEXTSEL_ADC345_TIM20_TRGO2                 (17U)   // 0b10001
#define ADC_JSQR_JEXTSEL_ADC345_TIM20_CC2                   (18U)   // 0b10010
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG2              (19U)   // 0b10011
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG4              (20U)   // 0b10100
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG5              (21U)   // 0b10101
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG6              (22U)   // 0b10110
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG7              (23U)   // 0b10111
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG8              (24U)   // 0b11000
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG9              (25U)   // 0b11001
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG10             (26U)   // 0b11010
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG1              (27U)   // 0b11011
#define ADC_JSQR_JEXTSEL_ADC345_HRTIM_ADC_TRG3              (28U)   // 0b11100
#define ADC_JSQR_JEXTSEL_ADC345_LPTIMOUT                    (29U)   // 0b11101
#define ADC_JSQR_JEXTSEL_ADC345_TIM7_TRGO                   (30U)   // 0b11110

/*! Proper definitions of ADC CFGR2 OVSS oversampling shift selection options */
#define ADC_CFGR2_OVSS_0_BIT            (0b0000U)
#define ADC_CFGR2_OVSS_1_BIT            (0b0001U)
#define ADC_CFGR2_OVSS_2_BIT            (0b0010U)
#define ADC_CFGR2_OVSS_3_BIT            (0b0011U)
#define ADC_CFGR2_OVSS_4_BIT            (0b0100U)
#define ADC_CFGR2_OVSS_5_BIT            (0b0101U)
#define ADC_CFGR2_OVSS_6_BIT            (0b0110U)
#define ADC_CFGR2_OVSS_7_BIT            (0b0111U)
#define ADC_CFGR2_OVSS_8_BIT            (0b1000U)

/*! Proper definitions of ADC CFGR2 OVSR oversampling ratio selection options */
#define ADC_CFGR2_OVSR_2x               (0b000U)
#define ADC_CFGR2_OVSR_4x               (0b001U)
#define ADC_CFGR2_OVSR_8x               (0b010U)
#define ADC_CFGR2_OVSR_16x              (0b011U)
#define ADC_CFGR2_OVSR_32x              (0b100U)
#define ADC_CFGR2_OVSR_64x              (0b101U)
#define ADC_CFGR2_OVSR_128x             (0b110U)
#define ADC_CFGR2_OVSR_256x             (0b111U)

/*! Proper definitions of ADC CCR DUAL dual mode selection options */
#define ADC_CCR_DUAL_INDEPENDENT                    (0b00000U)
#define ADC_CCR_DUAL_REG_INJ                        (0b00001U)
#define ADC_CCR_DUAL_RER_ALT_TRIG                   (0b00010U)
#define ADC_CCR_DUAL_INTERLEAVED_INJ                (0b00011U)
#define ADC_CCR_DUAL_INJ                            (0b00101U)
#define ADC_CCR_DUAL_REG                            (0b00110U)
#define ADC_CCR_DUAL_INTERLEAVED                    (0b00111U)
#define ADC_CCR_DUAL_ALT_TRIG                       (0b01001U)

/*! Proper definitions for ADC CCR PRESC prescaler selection options */
#define ADC_CCR_PRESC_NO_DIV            (0b0000U)
#define ADC_CCR_PRESC_DIV_BY_2          (0b0001U)
#define ADC_CCR_PRESC_DIV_BY_4          (0b0010U)
#define ADC_CCR_PRESC_DIV_BY_6          (0b0011U)
#define ADC_CCR_PRESC_DIV_BY_8          (0b0100U)
#define ADC_CCR_PRESC_DIV_BY_10         (0b0101U)
#define ADC_CCR_PRESC_DIV_BY_12         (0b0110U)
#define ADC_CCR_PRESC_DIV_BY_16         (0b0111U)
#define ADC_CCR_PRESC_DIV_BY_32         (0b1000U)
#define ADC_CCR_PRESC_DIV_BY_64         (0b1001U)
#define ADC_CCR_PRESC_DIV_BY_128        (0b1010U)
#define ADC_CCR_PRESC_DIV_BY_256        (0b1011U)

/****************************************************************************************/
/*                                                                                      */
/*                              OPAMP enhancement definitions                           */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of OPAMP CSR inverting input selection options */
#define OPAMP_CSR_VMSEL_VINM0                       (0b00U)
#define OPAMP_CSR_VMSEL_VINM1                       (0b01U)
#define OPAMP_CSR_VMSEL_Feedback_Resistor           (0b10U)
#define OPAMP_CSR_VMSEL_OPAMP_OUT                   (0b11U)

/*! Proper definitions of OPAMP CSR non inverted input selection options */
#define OPAMP_CSR_VPSEL_VINP0                       (0b00U)
#define OPAMP_CSR_VPSEL_VINP1                       (0b01U)
#define OPAMP_CSR_VPSEL_VINP2                       (0b10U)
#define OPAMP_CSR_VPSEL_Internal                    (0b11U)

/*! Proper definitions of OPAMP CSR calibration selection options */
#define OPAMP_CSR_CALSEL_0p033_VDDA                 (0b00U)
#define OPAMP_CSR_CALSEL_0p1_VDDA                   (0b01U)
#define OPAMP_CSR_CALSEL_0p5_VDDA                   (0b10U)
#define OPAMP_CSR_CALSEL_0p9_VDDA                   (0b11U)

/****************************************************************************************/
/*                                                                                      */
/*                              CORDIC enhancement definitions                          */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions of CORDIC CSR function selection options */
#define CORDIC_CSR_FUNC_Cosine                      (0U)
#define CORDIC_CSR_FUNC_Sine                        (1U)
#define CORDIC_CSR_FUNC_Phase                       (2U)
#define CORDIC_CSR_FUNC_Modulus                     (3U)
#define CORDIC_CSR_FUNC_Arctangent                  (4U)
#define CORDIC_CSR_FUNC_Hyb_Cosine                  (5U)
#define CORDIC_CSR_FUNC_Hyb_Sine                    (6U)
#define CORDIC_CSR_FUNC_Arctanh                     (7U)
#define CORDIC_CSR_FUNC_Natural_Log                 (8U)
#define CORDIC_CSR_FUNC_Square_Root                 (9U)

// @todo It might not be necessary in this place.

/****************************************************************************************/
/*                                                                                      */
/*                              FDCAN enhancement definitions                           */
/*                                                                                      */
/****************************************************************************************/

/*! Proper definitions for FDCAN RXGFC Accept Non-matching frames standard */
#define FDCAN_RXGFC_ANFS_TO_RXFIFO                  (0b00U)
#define FDCAN_RXGFC_ANFS_TO_RXFIF1                  (0b01U)
#define FDCAN_RXGFC_ANFS_REJECT                     (0b10U)

/*! Proper definitions for FDCAN RXGFC Accept Non-matching frames extended */
#define FDCAN_RXGFC_ANFE_TO_RXFIFO                  (0b00U)
#define FDCAN_RXGFC_ANFE_TO_RXFIF1                  (0b01U)
#define FDCAN_RXGFC_ANFE_REJECT                     (0b10U)


/**************************************************************************************************/
/*                                                                                                */
/*                            FDCAN Memory RAM enhancement definitions                            */
/*                                                                                                */
/**************************************************************************************************/

#define FDCAN1_MSGRAM                   (SRAMCAN_BASE)
#define FDCAN2_MSGRAM                   (SRAMCAN_BASE + 0x0354U)
#define FDCAN3_MSGRAM                   (SRAMCAN_BASE + 0x06A4U)

#define FDCAN_Standard_Filter_OFFSET    (0x0000U)
#define FDCAN_Extended_Filter_OFFSET    (0x0070U)
#define FDCAN_RXFIFO0_OFFSET            (0x00B0U)
#define FDCAN_RXFIFO1_OFFSET            (0x0188U)
#define FDCAN_TXeFIFO_OFFSET            (0x0260U)
#define FDCAN_TXFIFOQ_OFFSET            (0x0278U)

/**************************************************************************************************/
/*                                                                                                */
/*                            FDCAN definitions below are not used now                            */
/*                                                                                                */
/**************************************************************************************************/

typedef struct {
    __IO uint32_t ARB; // CAN frame arbitration field

    __IO uint32_t CNTRL; // CAN frame control field

    __IO uint8_t DATA[64]; // CAN frame data field
}FDCAN_FIFO_TypeDef;


typedef struct {
    __IO uint32_t ARB; // CAN frame arbitration field

    __IO uint32_t CNTRL; // CAN frame control field
}FDCAN_TXeFIFO_TypeDef;


typedef struct
{
    __IO uint32_t SF[28];   // Standart(11-bit) ID Filter
    __IO uint64_t EF[8];    // Extended(29-bit) ID Filter
} FDCAN_IDFLTR_TypeDef;


//typedef struct
//{
//    __IO uint32_t SF[28];
//    __IO uint64_t EF[8];
//    __IO FDCAN_FIFO_TypeDef RXFIFO0[3];
//    __IO FDCAN_FIFO_TypeDef RXFIFO1[3];
//    __IO FDCAN_TXeFIFO_TypeDef TXeFIFO;
//    __IO FDCAN_FIFO_TypeDef TXFIFOQ[3];
//} FDCAN_MSGRAM_TypeDef;
#define FDCAN1_IDFLTR                   ((FDCAN_FILTER_TypeDef *) FDCAN1_MSGRAM)
#define FDCAN1_RXFIFO0                  ((FDCAN_FIFO_TypeDef *) (FDCAN1_MSGRAM + FDCAN_RXFIFO0_OFFSET))
#define FDCAN1_RXFIFO1                  ((FDCAN_FIFO_TypeDef *) (FDCAN1_MSGRAM + FDCAN_RXFIFO1_OFFSET))
#define FDCAN1_TXeFIFO                  ((FDCAN_TXeFIFO_TypeDef *) (FDCAN1_MSGRAM + FDCAN_TXeFIFO_OFFSET))
#define FDCAN1_TXFIFOQ                  ((FDCAN_FIFO_TypeDef *) (FDCAN1_MSGRAM + FDCAN_TXFIFOQ_OFFSET))

#define FDCAN2_IDFLTR                   ((FDCAN_FILTER_TypeDef *) FDCAN2_MSGRAM)
#define FDCAN2_RXFIFO0                  ((FDCAN_FIFO_TypeDef *) (FDCAN2_MSGRAM + FDCAN_RXFIFO0_OFFSET))
#define FDCAN2_RXFIFO1                  ((FDCAN_FIFO_TypeDef *) (FDCAN2_MSGRAM + FDCAN_RXFIFO1_OFFSET))
#define FDCAN2_TXeFIFO                  ((FDCAN_TXeFIFO_TypeDef *) (FDCAN2_MSGRAM + FDCAN_TXeFIFO_OFFSET))
#define FDCAN2_TXFIFOQ                  ((FDCAN_FIFO_TypeDef *) (FDCAN2_MSGRAM + FDCAN_TXFIFOQ_OFFSET))

#define FDCAN3_IDFLTR                   ((FDCAN_FILTER_TypeDef *) FDCAN3_MSGRAM)
#define FDCAN3_RXFIFO0                  ((FDCAN_FIFO_TypeDef *) (FDCAN3_MSGRAM + FDCAN_RXFIFO0_OFFSET))
#define FDCAN3_RXFIFO1                  ((FDCAN_FIFO_TypeDef *) (FDCAN3_MSGRAM + FDCAN_RXFIFO1_OFFSET))
#define FDCAN3_TXeFIFO                  ((FDCAN_TXeFIFO_TypeDef *) (FDCAN3_MSGRAM + FDCAN_TXeFIFO_OFFSET))
#define FDCAN3_TXFIFOQ                  ((FDCAN_FIFO_TypeDef *) (FDCAN3_MSGRAM + FDCAN_TXFIFOQ_OFFSET))

/********************  Bit definition for FDCAN_IDFLTR SF *******************/
#define FDCAN_IDFLTR_SF_SFT_Pos                     (30U)
#define FDCAN_IDFLTR_SF_SFT_Msk                     (0b11UL << FDCAN_IDFLTR_SF_SFT_Pos)
#define FDCAN_IDFLTR_SF_SFT                         FDCAN_IDFLTR_SF_SFT_Msk
#define FDCAN_IDFLTR_SF_SFEC_Pos                    (27U)
#define FDCAN_IDFLTR_SF_SFEC_Msk                    (0b111UL << FDCAN_IDFLTR_SF_SFEC_Pos)
#define FDCAN_IDFLTR_SF_SFEC                        FDCAN_IDFLTR_SF_SFEC_Msk
#define FDCAN_IDFLTR_SF_SFID1_Pos                   (16U)
#define FDCAN_IDFLTR_SF_SFID1_Msk                   (0x7FFUL << FDCAN_IDFLTR_SF_SFID1_Pos)
#define FDCAN_IDFLTR_SF_SFID1                       FDCAN_IDFLTR_SF_SFID1_Msk
#define FDCAN_IDFLTR_SF_SFID2_Pos                   (0U)
#define FDCAN_IDFLTR_SF_SFID2_Msk                   (0x7FFUL << FDCAN_IDFLTR_SF_SFID2_Pos)
#define FDCAN_IDFLTR_SF_SFID2                       FDCAN_IDFLTR_SF_SFID2_Msk

/*! Proper definitions for FDCAN_IDFLT Standard Filter type */
#define FDCAN_IDFLTR_SF_SFT_Range                   (0b00U)
#define FDCAN_IDFLTR_SF_SFT_Dual_ID                 (0b01U)
#define FDCAN_IDFLTR_SF_SFT_Mask                    (0b10U) // @todo what is better "Mask" or "Classic filter"

/*! Proper definitions for FDCAN_IDFLT Standard Filter element configuration */
#define FDCAN_IDFLTR_SF_SFEC_TO_RXFIFO0             (0b001U)
#define FDCAN_IDFLTR_SF_SFEC_TO_RXFIFO1             (0b010U)
#define FDCAN_IDFLTR_SF_SFEC_REJECT_ID              (0b011U)
#define FDCAN_IDFLTR_SF_SFEC_HPM                    (0b100U)
#define FDCAN_IDFLTR_SF_SFEC_TO_RXFIFO0_HPM         (0b101U)
#define FDCAN_IDFLTR_SF_SFEC_TO_RXFIFO1_HPM         (0b110U)

/********************  Bit definition for FDCAN_IDFLTR EF *******************/
#define FDCAN_IDFLTR_EF_EFEC_Pos                    (61U)
#define FDCAN_IDFLTR_EF_EFEC_Msk                    (0b111ULL << FDCAN_IDFLTR_EF_EFEC_Pos)
#define FDCAN_IDFLTR_EF_EFEC                        FDCAN_IDFLTR_EF_EFEC_Msk
#define FDCAN_IDFLTR_EF_EFID1_Pos                   (32U)
#define FDCAN_IDFLTR_EF_EFID1_Msk                   (0x1FFFFFFFULL << FDCAN_IDFLTR_EF_EFID1_Pos)
#define FDCAN_IDFLTR_EF_EFID1                       FDCAN_IDFLTR_EF_EFID1_Msk
#define FDCAN_IDFLTR_EF_EFT_Pos                     (30U)
#define FDCAN_IDFLTR_EF_EFT_Msk                     (0b11ULL << FDCAN_IDFLTR_EF_EFT_Pos)
#define FDCAN_IDFLTR_EF_EFT                         FDCAN_IDFLTR_EF_EFT_Msk
#define FDCAN_IDFLTR_EF_EFID2_Pos                   (0U)
#define FDCAN_IDFLTR_EF_EFID2_Msk                   (0x1FFFFFFFULL << FDCAN_IDFLTR_EF_EFID2_Pos)
#define FDCAN_IDFLTR_EF_EFID2                       FDCAN_IDFLTR_EF_EFID2_Msk

/*! Proper definitions for FDCAN_IDFLT Extended Filter type*/
#define FDCAN_IDFLTR_EF_EFT_Range_XIDAM             (0b00U)
#define FDCAN_IDFLTR_EF_EFT_Dual_ID                 (0b01U)
#define FDCAN_IDFLTR_EF_EFT_Mask                    (0b10U) // @todo what is better "Mask" or "Classic filter"
#define FDCAN_IDFLTR_EF_EFT_Range                   (0b11U)

/*! Proper definitions for FDCAN_IDFLT Extended Filter element configuration */
#define FDCAN_IDFLTR_EF_EFEC_TO_RXFIFO0             (0b001U)
#define FDCAN_IDFLTR_EF_EFEC_TO_RXFIFO1             (0b010U)
#define FDCAN_IDFLTR_EF_EFEC_REJECT_ID              (0b011U)
#define FDCAN_IDFLTR_EF_EFEC_HPM                    (0b100U)
#define FDCAN_IDFLTR_EF_EFEC_TO_RXFIFO0_HPM         (0b101U)
#define FDCAN_IDFLTR_EF_EFEC_TO_RXFIFO1_HPM         (0b110U)

/********************  Bit definition for FDCAN_RXFIFO ARB *******************/
#define FDCAN_RXFIFO_ARB_ESI_Pos                    (31U)
#define FDCAN_RXFIFO_ARB_ESI_Msk                    (0b1UL << FDCAN_RXFIFO_ARB_ESI_Pos)
#define FDCAN_RXFIFO_ARB_ESI                        FDCAN_RXFIFO_ARB_ESI_Msk
#define FDCAN_RXFIFO_ARB_XTD_Pos                    (30U)
#define FDCAN_RXFIFO_ARB_XTD_Msk                    (0b1UL << FDCAN_RXFIFO_ARB_XTD_Pos)
#define FDCAN_RXFIFO_ARB_XTD                        FDCAN_RXFIFO_ARB_XTD_Msk
#define FDCAN_RXFIFO_ARB_RTR_Pos                    (29U)
#define FDCAN_RXFIFO_ARB_RTR_Msk                    (0b1UL << FDCAN_RXFIFO_ARB_RTR_Pos)
#define FDCAN_RXFIFO_ARB_RTR                        FDCAN_RXFIFO_ARB_RTR_Msk
#define FDCAN_RXFIFO_ARB_ID_Pos                     (0U)
#define FDCAN_RXFIFO_ARB_ID_Msk                     (0x1FFFFFFFUL << FDCAN_RXFIFO_ARB_ID_Pos)
#define FDCAN_RXFIFO_ARB_ID                         FDCAN_RXFIFO_ARB_ID_Msk

/********************  Bit definition for FDCAN_RXFIFO CNTRL *******************/
#define FDCAN_RXFIFO_CNTRL_ANMF_Pos                 (31U)
#define FDCAN_RXFIFO_CNTRL_ANMF_Msk                 (0b1UL << FDCAN_RXFIFO_CNTRL_ANMF_Pos)
#define FDCAN_RXFIFO_CNTRL_ANMF                     FDCAN_RXFIFO_CNTRL_ANMF_Msk
#define FDCAN_RXFIFO_CNTRL_FIDX_Pos                 (24U)
#define FDCAN_RXFIFO_CNTRL_FIDX_Msk                 (0b111111UL << FDCAN_RXFIFO_CNTRL_FIDX_Pos)
#define FDCAN_RXFIFO_CNTRL_FIDX                     FDCAN_RXFIFO_CNTRL_FIDX_Msk
#define FDCAN_RXFIFO_CNTRL_FDF_Pos                  (22U)
#define FDCAN_RXFIFO_CNTRL_FDF_Msk                  (0b1UL << FDCAN_RXFIFO_CNTRL_FDF_Pos)
#define FDCAN_RXFIFO_CNTRL_FDF                      FDCAN_RXFIFO_CNTRL_FDF_Msk
#define FDCAN_RXFIFO_CNTRL_BRS_Pos                  (21U)
#define FDCAN_RXFIFO_CNTRL_BRS_Msk                  (0b1UL << FDCAN_RXFIFO_CNTRL_BRS_Pos)
#define FDCAN_RXFIFO_CNTRL_BRS                      FDCAN_RXFIFO_CNTRL_BRS_Msk
#define FDCAN_RXFIFO_CNTRL_DLC_Pos                  (16U)
#define FDCAN_RXFIFO_CNTRL_DLC_Msk                  (0b1111UL << FDCAN_RXFIFO_CNTRL_DLC_Pos)
#define FDCAN_RXFIFO_CNTRL_DLC                      FDCAN_RXFIFO_CNTRL_DLC_Msk
#define FDCAN_RXFIFO_CNTRL_RXTS_Pos                 (0U)
#define FDCAN_RXFIFO_CNTRL_RXTS_Msk                 (0x7FFFUL << FDCAN_RXFIFO_CNTRL_RXTS_Pos)
#define FDCAN_RXFIFO_CNTRL_RXTS                     FDCAN_RXFIFO_CNTRL_RXTS_Msk

/********************  Bit definition for FDCAN_TXFIFOQ ARB *******************/
#define FDCAN_TXFIFOQ_ARB_ESI_Pos                    (31U)
#define FDCAN_TXFIFOQ_ARB_ESI_Msk                    (0b1UL << FDCAN_TXFIFOQ_ARB_ESI_Pos)
#define FDCAN_TXFIFOQ_ARB_ESI                        FDCAN_RXFIFO_ARB_ESI_Msk
#define FDCAN_TXFIFOQ_ARB_XTD_Pos                    (30U)
#define FDCAN_TXFIFOQ_ARB_XTD_Msk                    (0b1UL << FDCAN_TXFIFOQ_ARB_XTD_Pos)
#define FDCAN_TXFIFOQ_ARB_XTD                        FDCAN_RXFIFO_ARB_XTD_Msk
#define FDCAN_TXFIFOQ_ARB_RTR_Pos                    (29U)
#define FDCAN_TXFIFOQ_ARB_RTR_Msk                    (0b1UL << FDCAN_TXFIFOQ_ARB_RTR_Pos)
#define FDCAN_TXFIFOQ_ARB_RTR                        FDCAN_RXFIFO_ARB_RTR_Msk
#define FDCAN_TXFIFOQ_ARB_ID_Pos                     (0U)
#define FDCAN_TXFIFOQ_ARB_ID_Msk                     (0x1FFFFFFFUL << FDCAN_TXFIFOQ_ARB_ID_Pos)
#define FDCAN_TXFIFOQ_ARB_ID                         FDCAN_RXFIFO_ARB_ID_Msk

/********************  Bit definition for FDCAN_TXFIFOQ CNTRL *******************/
#define FDCAN_TXFIFOQ_CNTRL_MM_Pos                   (24U)
#define FDCAN_TXFIFOQ_CNTRL_MM_Msk                   (0xFFUL << FDCAN_TXFIFOQ_CNTRL_MM_Pos)
#define FDCAN_TXFIFOQ_CNTRL_MM                       FDCAN_TXFIFOQ_CNTRL_MM_Msk
#define FDCAN_TXFIFOQ_CNTRL_EFC_Pos                  (23U)
#define FDCAN_TXFIFOQ_CNTRL_EFC_Msk                  (0b1UL << FDCAN_TXFIFOQ_CNTRL_EFC_Pos)
#define FDCAN_TXFIFOQ_CNTRL_EFC                      FDCAN_TXFIFOQ_CNTRL_EFC_Msk
#define FDCAN_TXFIFOQ_CNTRL_FDF_Pos                  (21U)
#define FDCAN_TXFIFOQ_CNTRL_FDF_Msk                  (0b1UL << FDCAN_TXFIFOQ_CNTRL_FDF_Pos)
#define FDCAN_TXFIFOQ_CNTRL_FDF                      FDCAN_TXFIFOQ_CNTRL_FDF_Msk
#define FDCAN_TXFIFOQ_CNTRL_BPS_Pos                  (20U)
#define FDCAN_TXFIFOQ_CNTRL_BPS_Msk                  (0b1UL << FDCAN_TXFIFOQ_CNTRL_BPS_Pos)
#define FDCAN_TXFIFOQ_CNTRL_BPS                      FDCAN_TXFIFOQ_CNTRL_BPS_Msk
#define FDCAN_TXFIFOQ_CNTRL_DLC_Pos                  (16U)
#define FDCAN_TXFIFOQ_CNTRL_DLC_Msk                  (0b1111UL << FDCAN_TXFIFOQ_CNTRL_DLC_Pos)
#define FDCAN_TXFIFOQ_CNTRL_DLC                      FDCAN_TXFIFOQ_CNTRL_DLC_Msk

/********************  Bit definition for FDCAN_TXeFIFO ARB *******************/
#define FDCAN_TXeFIFO_ARB_ESI_Pos                   (31U)
#define FDCAN_TXeFIFO_ARB_ESI_Msk                   (0b1UL << FDCAN_TXeFIFO_ARB_ESI_Pos)
#define FDCAN_TXeFIFO_ARB_ESI                       FDCAN_RXFIFO_ARB_ESI_Msk
#define FDCAN_TXeFIFO_ARB_XTD_Pos                   (30U)
#define FDCAN_TXeFIFO_ARB_XTD_Msk                   (0b1UL << FDCAN_TXeFIFO_ARB_XTD_Pos)
#define FDCAN_TXeFIFO_ARB_XTD                       FDCAN_RXFIFO_ARB_XTD_Msk
#define FDCAN_TXeFIFO_ARB_RTR_Pos                   (29U)
#define FDCAN_TXeFIFO_ARB_RTR_Msk                   (0b1UL << FDCAN_TXeFIFO_ARB_RTR_Pos)
#define FDCAN_TXeFIFO_ARB_RTR                       FDCAN_RXFIFO_ARB_RTR_Msk
#define FDCAN_TXeFIFO_ARB_ID_Pos                    (0U)
#define FDCAN_TXeFIFO_ARB_ID_Msk                    (0x1FFFFFFFUL << FDCAN_TXeFIFO_ARB_ID_Pos)
#define FDCAN_TXeFIFO_ARB_ID                        FDCAN_RXFIFO_ARB_ID_Msk

/********************  Bit definition for FDCAN_TXeFIFO CNTRL *******************/
#define FDCAN_TXeFIFO_CNTRL_MM_Pos                  (24U)
#define FDCAN_TXeFIFO_CNTRL_MM_Msk                  (0xFFUL << FDCAN_TXeFIFO_CNTRL_MM_Pos)
#define FDCAN_TXeFIFO_CNTRL_MM                      FDCAN_TXeFIFO_CNTRL_MM_Msk
#define FDCAN_TXeFIFO_CNTRL_ET_Pos                  (22U)
#define FDCAN_TXeFIFO_CNTRL_ET_Msk                  (0b11UL << FDCAN_TXeFIFO_CNTRL_ET_Pos)
#define FDCAN_TXeFIFO_CNTRL_ET                      FDCAN_TXeFIFO_CNTRL_ET_Msk
#define FDCAN_TXeFIFO_CNTRL_EDL_Pos                 (21U)
#define FDCAN_TXeFIFO_CNTRL_EDL_Msk                 (0b1UL << FDCAN_TXeFIFO_CNTRL_EDL_Pos)
#define FDCAN_TXeFIFO_CNTRL_EDL                     FDCAN_TXeFIFO_CNTRL_EDL_Msk
#define FDCAN_TXeFIFO_CNTRL_BRS_Pos                 (20U)
#define FDCAN_TXeFIFO_CNTRL_BRS_Msk                 (0b1UL << FDCAN_TXeFIFO_CNTRL_BRS_Pos)
#define FDCAN_TXeFIFO_CNTRL_BRS                     FDCAN_TXeFIFO_CNTRL_BRS_Msk
#define FDCAN_TXeFIFO_CNTRL_DLC_Pos                 (16U)
#define FDCAN_TXeFIFO_CNTRL_DLC_Msk                 (0b1111UL << FDCAN_TXeFIFO_CNTRL_DLC_Pos)
#define FDCAN_TXeFIFO_CNTRL_DLC                     FDCAN_TXeFIFO_CNTRL_DLC_Msk
#define FDCAN_TXeFIFO_CNTRL_TXTS_Pos                (0U)
#define FDCAN_TXeFIFO_CNTRL_TXTS_Msk                (0xFFFFUL << FDCAN_TXeFIFO_CNTRL_TXTS_Pos)
#define FDCAN_TXeFIFO_CNTRL_TXTS                    FDCAN_TXeFIFO_CNTRL_TXTS_Msk

/*! Proper definitions for FDCAN_TXeFIFO_CNTRL Event type */
#define FDCAN_TXeFIFO_CNTRL_EFC_TX_EVENT            (0b01U)
#define FDCAN_TXeFIFO_CNTRL_EFC_IGNORE_CANCEL       (0b10U)




#endif /* CMSIS_ENHANCEMENT_H_ */

/**
 * These constants are the same for most of STM32 families and are very simple. I think these should be included into the CMSIS by default, but they are not, therefore we store them in a single file
 */
