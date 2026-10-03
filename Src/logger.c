#include "logger.h"
#include "usart.h"

struct usart_clock_config logger_clock_config = {
		.clock_source = RCC_PCKL1,
		.prescaler = PRESCALE_BY_1,
};

struct usart_config logger_kernel_config = {
		.baud_rate = USART_BAUD_RATE_9600,
		.instance = USART_INSTANCE_3,
		.is_fifo_en = 0,
		.is_parity_enable = 0,
		.parity = USART_EVEN_PARITY,
		.stop_bits = USART_STOP_BITS_1,
		.word_length = USART_DATA_BITS_8,
		.noise_config = USART_NOISE_DETECTION_ENABLE,
};


void logger_init(void) {
	usart_init(USART_INSTANCE_3, &logger_clock_config);
	usart_config(&logger_kernel_config, &logger_clock_config);
}

int __io_putchar(int ch) {
	uint8_t byte = (uint8_t)ch;
	usart_transmit_data(&byte, 1);
	return ch;
}
