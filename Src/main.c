#include <usart.h>
#include "logger.h"
#include <stdio.h>
#define BUFFER_SIZE 1024

void usart_example_process_action(void);

struct usart_clock_config clock_config = {
		.clock_source = RCC_PCLK,
		.prescaler = PRESCALE_BY_1,
};

struct usart_config usart_kernel_config = {
		.baud_rate = USART_BAUD_RATE_9600,
		.instance = USART_INSTANCE_1,
		.is_fifo_en = 0,
		.is_parity_enable = 1,
		.parity = USART_EVEN_PARITY,
		.stop_bits = USART_STOP_BITS_1,
		.word_length = USART_DATA_BITS_8,
		.noise_config = USART_NOISE_DETECTION_ENABLE,
};

USART_TypeDef *instance_handle;

uint8_t transmit_buffer[BUFFER_SIZE];
uint8_t receive_buffer[BUFFER_SIZE];

usart_mode current_mode = USART_TRANSMIT;

int main() {
	logger_init();
	printf("Starting USART Example Application\n");

	instance_handle = usart_get_instance_handle(usart_kernel_config.instance);
	usart_init(usart_kernel_config.instance, &clock_config);
	printf("USART initialized successfully\n");
	usart_config(&usart_kernel_config, &clock_config, instance_handle);
	printf("USART configuration set successfully\n");
	// initialization and configuration done
	while(1) {
		usart_example_process_action();
	}
}

void usart_example_process_action(void) {
	switch(current_mode) {
	case USART_TRANSMIT:
		for(uint16_t i=0;i<BUFFER_SIZE;i++) {
			transmit_buffer[i] = (uint8_t)(i+1);
		}
		usart_transmit_data(transmit_buffer, sizeof(transmit_buffer)/sizeof(uint8_t), instance_handle, 0);
		printf("Data transmitted successfully\n");
		break;


	case USART_RECEIVE:
		usart_status status = usart_receive_data(receive_buffer, sizeof(receive_buffer)/sizeof(uint8_t), instance_handle);
		if (status != STATUS_OK) {
			printf("Receive error detected\n");
		}
		break;

	default:
		break;
	}
}

