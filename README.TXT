This is C library to use NRF24L01+ module with any AVR uC which has built in SPI protocol.

/* 
 * If you appreciate my work and like the library you can make some small donation on paypal:
 * paypal email : havahijakim@gmail.com
 *
 * If you have any questions feel free to ask : 
 * Email : havahijakim@gmail.com 
 * Email title : NRF24 library
 *
 * This program is free software; you can redistribute it and/or
 *     modify it unless it is used for business purposes.
 */

Examples of uC : ATMEGA328P, ATMEGA2560, ATTINY85, ATTINY84, ATMEGA32

Library is already configured for ATMEGA328P / ARDUINO.
Library is plug and play for ARDUINO. No configuration needed.

If you use different uC than ATMEGA328P it is crucial to configure SPI protocol. 
You'll find how to do this in nrf24.h file

Before staring study nrf24.h file for a while
Every single command is explained in nrf24.h file

Library is configured to use autoacknowledgment function as well as dynamic payloads. 
Library doesn't support turning off auto ACK or setting static payloads.
Library also supports auto acknowledgment /w payloads, so you don't have to switch between RX and TX mode in a 
single module.
Library has rigidly set 5 bytes addresses.
Library has rigidly set 2 bytes CRC encoding.
Library supports multiceiver mode.
IRQ pin is enabled for RX_DR, TX_DS and MAX_RT.

In examples folder you will find main.c files for each transmitter and receiver.

You will find examples:
-communication with ACK without ACK_payload
-communication with ACK with ACK_payload
-communication in multiceiver mode with reading which pipe payload came from
-communication looped until transmitter receive ACK with or without payload

Library also supports low power modes for projects made on batteries.

To use this library you only need to know 1 thing: 
---------------------------------------------------------------------------------------------------------
NRF24 has its own STATUS register, which is basically 3 bits.
		RX bit, TX bit, MAX_RT bit
RX_bit == 1 when receiver receive new payload 
TX_bit == 1 when transmitter receive acknowledgment
MAX_RT == 1 when transmitter doesn't receive any acknowledgment

To reset all of those bits just simply use nrf24_resetstatus() command.

If you don't understand this don't worry. Every thing is better explained in nrf24.h file.
---------------------------------------------------------------------------------------------------------
I recommend reading NRF24L01+ datasheet to better understand how to works and what is happening

If examples sketch is uploaded to uC and doesn't work first thing to do is adding at least 100uF capacitor
between VCC and GND of NRF24 module, as sometimes it needs high current spikes when changing to some modes.
Arduino without capacitor can't provide enough power resulting in problems as no ACK or constantly resetting
NRF24 module to default factory settings.
