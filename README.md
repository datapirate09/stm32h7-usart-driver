# STM32H7 USART Driver

A register-level USART driver for the STM32H7A3 (NUCLEO-H7A3ZI-Q), implemented without HAL or CubeMX-generated peripheral code. Includes a reusable `printf`-retargeting logger component and a DMA-based non-blocking transmission path, built on modular USART and DMA drivers.

## Features

- Configurable USART instance, baud rate, word length, parity, and stop bits
- Support for configuring multiple USART instances
- Optional FIFO mode support
- Blocking (polling-based) transmission and reception
- Interrupt-driven transmission with transmit-complete callback support
- DMA-based non-blocking USART transmission
- DMA stream and DMAMUX request configuration through a reusable handle-based API
- Receive error detection for overrun, noise, and framing errors
- USART kernel clock source selection: PCLK, HSI kernel clock, CSI kernel clock, and LSE
- `printf` retargeting through a reusable logger implementation
- Modular driver architecture separating USART configuration, DMA configuration, and application-level logging

## Hardware

- **Board:** NUCLEO-H7A3ZI-Q
- **USART3:** PD8 (TX) / PD9 (RX), routed to the onboard ST-LINK virtual COM port

## Interrupt-Driven Transmission

- Uses USART status flags and interrupt-enable bits to feed data to the transmitter without blocking the calling code.
- Disables the transmit FIFO empty interrupt once the entire buffer has been queued.
- Uses the transmission-complete interrupt to detect when the final frame has finished transmitting.
- Supports a user-registered callback to notify the application when transmission completes.

## DMA-Based Transmission

- Integrates the DMA1 controller and DMAMUX to support non-blocking USART data transmission.
- Uses a handle-based API to associate a DMA stream with its corresponding DMAMUX channel.
- Configures DMA transfer parameters, including source and peripheral addresses, transfer length, priority, FIFO threshold, transfer direction, and peripheral request selection.
- Routes USART transmit requests through the DMAMUX to trigger DMA transfers to the USART transmit data register.
- Retargets `printf` output through the DMA-based transmission path, reducing CPU involvement in moving individual data elements to the USART peripheral.
- Separates DMA resource selection and transfer configuration from the USART driver, improving code reuse and maintainability.

## Debugging Notes

- **Corrupted `printf` output:** Output was silently corrupted despite apparently correct GPIO and clock configuration. The root cause was parity being enabled unconditionally in CR1. The word-length configuration must account for parity because the configured frame length includes the parity bit. This caused a mismatch with the terminal's 8N1 configuration. The issue was identified by comparing the driver against a known-working minimal polling implementation.
- **FIFO and non-FIFO status flags:** Initially assumed FIFO and non-FIFO modes used different status bits for transmit readiness. Confirmed in RM0455 that the relevant status bit position is shared, with its meaning depending on the FIFO enable configuration.
- **Interrupt-driven TX completion:** Transmission requires distinguishing between data being queued in the transmit FIFO and the final frame actually finishing on the TX line. The transmit FIFO empty interrupt handles feeding data, while the transmission-complete interrupt signals the end of the transfer.
- **DMA buffer lifetime:** DMA transfers access the source buffer independently of the calling function after a transfer is started. The source buffer must remain valid and unchanged until the transfer completes. Reconfiguring or reusing a DMA stream must also account for any transfer already in progress.