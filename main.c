#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

void clock_setup()
{
    /* Enable GPIOA clock. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_GPIOC);
//	rcc_set_i2c_clock_hsi(I2C1);
    rcc_get_i2c_clk_freq(I2C1_BASE);
}


void gpio_setup()
{
    /* put your i2c gpio setup here */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,              
           GPIO13);
//  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);
//	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8 | GPIO9);
//	gpio_set_af(GPIOB, GPIO_AF4, GPIO8 | GPIO9);
}


int main(void)
{
    clock_setup();
    gpio_setup();

    for(;;) {
            int i;
            gpio_toggle(GPIOC, GPIO13);
            for (i = 0; i < 800000; i++) {	/* Wait a bit. */
                    __asm__("nop");
           }
    }

}
