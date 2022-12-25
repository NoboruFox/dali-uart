#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <libopencm3/cm3/nvic.h>

#define SEND_BUFFER_SIZE    25

//int _write(int file, char *ptr, int len);
static void usart_send_string(uint32_t usart, uint8_t *string, uint16_t str_size);
static void usart_get_string(uint32_t usart, uint8_t *string, uint16_t str_max_size);

void clock_setup()
{
    /* Enable GPIOA clock. */
    rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_USART1);
}

void gpio_setup()
{
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
}

static void usart_setup(void)
{
	/* Setup UART parameters. */
	nvic_enable_irq(NVIC_USART1_IRQ);
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);

    usart_enable_rx_interrupt(USART1);
	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void usart_send_string(uint32_t usart, uint8_t *string, uint16_t str_size)
{
    uint16_t iter = 0;
    do
    {
        usart_send_blocking(usart, string[iter++]);
    }while(string[iter] != 0 && iter < str_size);
}

static void usart_get_string(uint32_t usart, uint8_t *out_string, uint16_t str_max_size)
{
    uint8_t sign = 0;
    uint16_t iter = 0;

    while(iter < str_max_size)
    {
        sign = usart_recv_blocking(usart);
        out_string[iter++] = sign;
    }
//    return iter;
}

int main(void)
{
    clock_setup();
    gpio_setup();
    usart_setup();

    while(1) 
    {
        __asm__("nop");
    }
    return 0;
}

void usart1_irw(void)
{
	uint8_t str;
    uint16_t iter = 0;
	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

		/* Indicate that we got data. */
		gpio_toggle(GPIOC, GPIO13);

		/* Retrieve the data from the peripheral. */
        str = usart_recv(USART1);
    
//        usart_enable_tx_interrupt(USART1);
		/* Enable transmit interrupt so it sends back the data. */
//	}

	/* Check if we were called because of TXE. */
//	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
//	    ((USART_SR(USART1) & USART_SR_TXE) != 0)) {

		/* Put data into the transmit register. */
        usart_send(USART1, str);

		gpio_toggle(GPIOC, GPIO13);

		/* Disable the TXE interrupt as we don't need it anymore. */
//		usart_disable_tx_interrupt(USART1);
	}
}

