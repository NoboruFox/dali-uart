#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <errno.h>

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
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);

//    usart_enable_rx_interrupt(USART2);
	/* Finally enable the USART. */
	usart_enable(USART1);
}

#if 0
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART1, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}
#endif

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
//        usart_send_blocking(usart, sign);

//        if(sign != '\n' && sign != '\r')
            out_string[iter++] = sign;
//        else
//        {
//            out_string[iter] = 0;
//            usart_send_string(USART1, (uint8_t*)"end\r\n", 6);
//            break;
//        }
    }
//    return iter;
}

int main(void)
{
    clock_setup();
    gpio_setup();
    usart_setup();

    int i;
    uint8_t str_send[SEND_BUFFER_SIZE];

    while(1) 
    {
        gpio_toggle(GPIOC, GPIO13);
//      printf("Hello, world\r\n"); //using _write in printf and redefining it to output to usart (using usart_send_blocking). Can use own function f.e. usart_send_string, and call it here directly
//        usart_send_string(USART1, (uint8_t*)"Hello\n\r", SEND_BUFFER_SIZE);
        usart_get_string(USART1, str_send, SEND_BUFFER_SIZE);
        usart_send_string(USART1, str_send, SEND_BUFFER_SIZE);
            for (i = 0; i < 800000; i++) {	/* Wait a bit. */
                    __asm__("nop");
            }       
    }

}
