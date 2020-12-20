#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "nrf24.h"


uint8_t address0[5] = {0x01, 0x02, 0x03, 0x04, 0x05}; //set 5 byte address of data pipe 0
uint8_t length; //variable to store length of payload
uint8_t received_data[32] = ""; //array to store received payload. Make sure that it is 32 bytes long and empty

int main(void){
	USART_Init();
	SPI_init();
	nrf24_configRX(address0); //configures module as receiver and enable data pipe 0
	nrf24_startlistening(); //starts listening for payloads
	while(1){
		if (nrf24_available()){ //if receiver received new payload
			length = nrf24_payloadlength(); //read length of new payload
			nrf24_read(received_data, length); //puts payload into received_data array and resets status register
		}
	}
}


