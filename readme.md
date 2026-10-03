# STM32H7 USART Driver

Register-level USART driver for the STM32H7A3 (Nucleo-H7A3ZI-Q), built without 
HAL or CubeMX-generated peripheral code. Includes a printf-retargeting logger 
component built on top of the driver. The logger component is reusable and can be
found in all the other projects.

## Features
- Configurable instance, baud rate, word length, parity, stop bits
- Optional FIFO mode support
- Blocking transmit/receive with error detection (overrun, noise, framing)
- Clock source selection for the USART kernel clock(PCLK1, HSI kernel clock, CSI kernel clock, LSE)
- printf retargeting via a logger implementation

## Hardware
- Board: NUCLEO-H7A3ZI-Q
- USART3 on PD8 (TX) / PD9 (RX), routed to the onboard ST-LINK virtual COM port

## Debugging notes
- Found that printf output was silently corrupted even though GPIO/clock 
  config looked correct. Root cause: parity was unconditionally enabled 
  in CR1, which shifts the effective data bit count under the hood (the 
  M bits define total frame size including parity), so frames no longer 
  matched the receiving terminal's 8N1 setting. Found by diffing against 
  a known-working minimal polling implementation.
- Initially assumed FIFO and non-FIFO modes used different status bits 
  (TXFNF vs TXE). Confirmed in RM0455 that they share the same bit 
  position, with hardware interpreting it differently based on FIFOEN (Fifo enable) - 
  simplified the driver accordingly.