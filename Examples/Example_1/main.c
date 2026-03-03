#include "rtos_tasks.h"
#include "device_mcu_includes.h"

void example_setup_leds(void);
void example_setup_buttons(void);

int main()
{

    example_setup_leds();
    example_setup_buttons();

    GPIOC->BSRR = GPIO_BSRR_BS14;

    vRtosSetup();

    while(1) {}

    return 0;
}

void example_setup_leds(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;


    uint32_t gpio_c_moder_clear_mask = ~(GPIO_MODER_MODE13_Msk | GPIO_MODER_MODE2_Msk
                                | GPIO_MODER_MODE3_Msk | GPIO_MODER_MODE0_Msk);
    uint32_t gpio_c_moder_set_mask = (GPIO_DIGITAL_OUT_Mode << GPIO_MODER_MODE13_Pos
                                | GPIO_DIGITAL_OUT_Mode << GPIO_MODER_MODE2_Pos
                                | GPIO_DIGITAL_OUT_Mode << GPIO_MODER_MODE3_Pos
                                | GPIO_DIGITAL_OUT_Mode << GPIO_MODER_MODE0_Pos);

    GPIOC->MODER = (GPIOC->MODER & gpio_c_moder_clear_mask) | gpio_c_moder_set_mask;
}

void example_setup_buttons(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;


    uint32_t gpio_b_moder_clear_mask = ~(GPIO_MODER_MODE3_Msk | GPIO_MODER_MODE4_Msk
                                | GPIO_MODER_MODE5_Msk);
    uint32_t gpio_b_moder_set_mask = (GPIO_DIGITAL_IN_Mode << GPIO_MODER_MODE3_Pos
                                | GPIO_DIGITAL_IN_Mode << GPIO_MODER_MODE4_Pos
                                | GPIO_DIGITAL_IN_Mode << GPIO_MODER_MODE5_Pos);

    GPIOB->MODER = (GPIOB->MODER & gpio_b_moder_clear_mask) | gpio_b_moder_set_mask;


    uint32_t gpio_b_pupdr_clear_mask = ~(GPIO_PUPDR_PUPD3_Msk | GPIO_PUPDR_PUPD4_Msk
                                | GPIO_PUPDR_PUPD5_Msk);
    uint32_t gpio_b_pupdr_set_mask = (GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPD3_Pos
                                | GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPD4_Pos
                                | GPIO_PUPDR_PULL_DOWN << GPIO_PUPDR_PUPD5_Pos);

    GPIOB->PUPDR = (GPIOB->PUPDR & gpio_b_pupdr_clear_mask) | gpio_b_pupdr_set_mask;
}
