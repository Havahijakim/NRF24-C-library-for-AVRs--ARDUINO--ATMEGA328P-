/*
 * 												NRF24 C LIBRARY FOR AVR uC
 *  Created on: 28 lis 2020
 *  Author: Hava Hijakim
 *
 *  This program is free software; you can redistribute it and/or
 *	modify it unless it is used for business purposes.
 *
 *	If you appreciate my work and like the library you can make some small donation on paypal:
 *	paypal email : havahijakim@gmail.com
 *
 *	If you have any questions feel free to ask : havahijakim@gmail.com
 *
 *	Study this entire file to understand every command below
 */

#ifndef NRF24_H_
#define NRF24_H_

////////////////////////////////////////////////////DEFINE OWN VALUES BELOW !!!//////////////////////////////////////////////////////////////////
/*
 *                                  All values are predefined for atmega328p or arduino !!!
 *
 *                                    			   NRF24 ---- ARDUINO
 *
 *                                    				VCC   ---- +3.3V
 *                                    				GND   ---- GND
 *                                    				CE    ---- pin 9 (PB1)
 *                                    				SCK   ---- pin 13 (PB5)
 *                                    				MISO  ---- pin 12 (PB4)
 *                                    				CSN   ---- pin 10 (PB2)
 *                                    				MOSI  ---- pin 11 (PB3)
 *
 *                         its recommended to add at least 100uF capacitor between VCC and GND of NRF24
 *                          as having no capacitor may result in problems especially on receiver's side
 *                                   		     while sending acknowledgment
 *
 */

//define SPI pins
#define MISO PB4  //pin MISO ex. PB4
#define MOSI PB3  //pin MOSI ex. PB3
#define SCK PB5  //pin SCK ex. PB5
#define CE PB1  //pin CE ex. PB1
#define CSN PB2  //pin CSN ex. PB2

//define SPI pins status registers
#define MISO_reg DDRB  //pin MISO register ex. DDRB
#define MOSI_reg DDRB  //pin MOSI register ex. DDRB
#define SCK_reg DDRB //pin SCK register ex. DDRB
#define CE_reg DDRB //pin CE register ex. DDRB
#define CSN_reg DDRB //pin CSN register ex. DDRB

//define SPI pins set registers
#define CE_set PORTB  //pin CE set register ex. PORTB
#define CSN_set PORTB  //pin CSN set register ex. PORTB

//define MISO pin read register
#define MISO_read PINB

//define SPI registers and bits
#define SPI_status_reg SPSR  //status register of SPI ex. SPSR
#define SPI_interrupt_flag SPIF //flag of SPI interrupt ex. SPIF
#define SPI_data_reg SPDR //SPI data register ex. SPDR

/*  		CONFIG SPI FOR YOUR uC !!!!!!
 * Go to NRF24.c and in void SPI_config() set:
 * -SPI Enable
 * -Data order MSB first
 * -Master Select
 * -SCK Polarity low when idle
 * -SCK Phase (leading edge-sample)(trailing edge-setup)
 * -SCK Frq (max 4Mhz)
 */

//////////////////////////////////////////////////END OF DEFINING OWN VALUES !!!///////////////////////////////////////////////////////////////

//NRF24 registers
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define RPD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D

//NRF24 commands
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define MASK_REGISTER 0x1F
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD 0xA8
#define W_TX_PAYLOAD_NOACK 0xB0
#define NOP 0xFF

//////////////////////////////////////////////////////INTIALIZING COMMANDS//////////////////////////////////////////////////////////////
void SPI_init();
/*
 *		   this command initializes SPI required to communication with NRF24 on uC
 * 				use this only once at uC startup or every time uC resets
 * 								ex. SPI_init();
 */

/////////////////////////////////////////////////////NRF24 CONFIG COMMANDS///////////////////////////////////////////////////////////////////////

void nrf24_configRX(unsigned char address[]);
/*
 *      used to configure NRF24 as receiver on pipe0
 * ex. uint8_t address0[5] = {0x00, 0x01, 0x02, 0x03, 0x04};
 * ex.        nrf24_configRX(address0);
 *
 * its it recommended to use this command only once, when
 * 				  NRF24 is turned on
 * to reuse this command it is recommended to restart NRF24
 *
 *    this command automatically activates data pipe 0
 *
 *    		    USE ONLY ON RECEIVER !!!
 */

void nrf24_configTX(unsigned char address[]);
/*
 *        used to configure NRF24 as transmitter
 * ex. uint8_t address0[5] = {0x00, 0x01, 0x02, 0x03, 0x04};
 * ex.        nrf24_configTX(address0);
 *
 * its it recommended to use this command only once, when
 * 				   NRF24 is turned on
 * to reuse this command it is recommended to restart NRF24
 *
 * 			    USE ONLY ON TRANSMITTER !!!
 */

void nrf24_RXaddr(unsigned char address[], int pipe);
/*
 *      it is used to enable rx data pipe 1-5 and assign address to the specified one
 *
 *            ex. uint8_t address1[5] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
 * 	 	                  ex. nrf24_RXaddr(address1, 1);
 *
 * first variable specifies data pipe address, while second specifies data pipe number (1..5)
 *
 * 				its it recommended to use this command only once, when
 * 				   				NRF24 is turned on
 * 				to reuse this command it is recommended to restart NRF24
 *
 * 	 if you use only data pipe 0 (no multiceiver) there is no need for this command
 *
 * 					        USE ONLY ON RECEIVER !!!
 */

void nrf24_TXaddr(unsigned char address[]);
/*
 *       		 it is used to assign address to TRANSMITTER
 *
 *          ex. uint8_t address1[5] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
 * 	 	               ex. nrf24_TXaddr(address1);
 *
 *			     its it recommended to use this command only once, when
 * 			    				 NRF24 is turned on
 * 			  to reuse this command it is recommended to restart NRF24
 *
 * 	 if you use only data pipe 0 (no multiceiver) there is no need for this command
 *
 * 					      USE ONLY ON TRANSMITTER !!!
 */



/////////////////////////////////////////////NRF24 POWER MANAGEMENT COMMANDS///////////////////////////////////////////////////////////////////////
void nrf24_pwrdown();
/*
 * this command is used to put nrf24 in sleep mode to obtain low power consumption (eg. 900nA)
 * 										ex. nrf24_pwrdown();
 * 							this command doesn't reset configuration of nrf24
 */

void nrf24_pwrupTX();
/*
 * this command is used to put nrf24 back into TX mode (standby-I) (eg. 30uA)
 * 					   ex. nrf24_pwrupTX();
 * 			    this command doesn't send any payload
 *
 *          always use command nrf24_TXaddr() after this
 * 			        USE ONLY ON TRANSMITTER !!!
 */

void nrf24_pwrupRX();
/*
 * this command is used to put nrf24 back into RX mode (standby-I) (eg. 30uA)
 * 					   ex. nrf24_pwrupRX();
 * 		     this command doesn't start listening for payload
 *
 *          always use command nrf24_RXaddr() after this
 * 		     		  USE ONLY ON RECEIVER !!!
 */



///////////////////////////////////////////////////NRF24 RECEIVER'S COMMANDS////////////////////////////////////////////////////////////////
void nrf24_startlistening();
/*
 * 		      this command puts receiver in listening mode
 *      if this command had been enabled receiver will receive payload
 *                    ex. nrf24_startlistening();
 *
 * in this mode receiver consumes between 10-15mA (depends on transmission speed)
 *
 * 					    USE ONLY ON RECEIVER !!!!
 */

void nrf24_stoplistening();
/*
 * 		           this command puts receiver in standby-I mode
 *      if this command had been enabled receiver will stop receiving payload
 *                       ex. nrf24_stoplistening();
 *
 *               in this mode receiver consumes around 30uA
 *
 * 					      USE ONLY ON RECEIVER !!!!
 */

void nrf24_writeACK(unsigned char ACK[], int pipe, int length);
/*
 *   this command is used to write ACK payload to be send back to transmitter via specified data pipe
 *                     max acknowledgment payload length is 32bytes
 *             pipe number is 0...5 (type 0 if you don't use multiceiver)
 *          length is length of acknowledgment payload. Using sizeof() is recommended
 *
 *        ex. uint8_t acknowledgment[32] = "this is acknowledgment extended ";
 *           ex. nrf24_writeACK(acknowledgment, 0, sizeof(acknowledgment);
 *
 *                               USE ONLY ON RECEIVER !!!!
 *
 */

int nrf24_available();
/*
 * this command returns 1 if new payload has been received
 *
 * 							ex.
 *
 * if (nrf24_available()){
 * 	//instruction you want to do when payload has been received
 * }
 *
 * 				   USE ONLY ON RECEIVER !!!!
 */

void nrf24_read(unsigned char payload[], int length);
/*
 * this command reads payload which waits in receiver's payload register
 *
 * 						   Usage with static arrays
 * ex. received_data[32] = ""; //always ensure that array is 32bytes and empty before reading
 * ex. nrf24_read(received_data, length); length is preset before reading operation with nrf24_payloadlength(); command
 *
 *                      Using nrf24_read command with dynamic arrays is impossible.
 * You can make received_data[32] array less than 32 bytes to save some memory unless you know exact payload size
 *
 *	          this command automatically resets status register so you don't have to do this manually
 */

int nrf24_payloadlength();
/*
 * this command returns length of payload which is in receiver's payload register
 *
 * 								ex. int length;
 * 					  ex. length = nrf24_payloadlength();
 *
 * respectively for 1 byte payload command returns 1; for 32 byte payload command returns 32
 *
 */
int nrf24_pipenum();
/*
 * this command returns number of data pipe assigned to pending payload
 * 			   use this command only in multiceiver mode
 *
 * 							ex. int pipe_number;
 * 					  ex. pipe_number = nrf24_pipenum();
 *
 * this command is helpful if you don't know the order of payloads received from different transmitters
 */
void nrf24_readmulticeiver(unsigned char payload[], int length);
/*
 *                               this command is used in multiceiver mode
 *                      nrf24 can store up to 3 payloads from 3 different pipes at the time
 *        if there is more than 1 payload pending in receiver's RX register use this command to read each one,
 *              instead of reading one and flushing RX register as nrf24_read() do (so you lose 1-2 pending ones)
 * Shortly use this if you don't want to loose any payload in RX register and you are not sure how many of them are in this register
 *											Recommended usage:
 *										for (int i=0; i<3; i++){
 *											if (nrf24_available()){
 *												//read pipe number
 *												//read length
 *												if (pipe == specified pipe){
 *													nrf24_readmulticeiver(variable, length);
 *												}
 *											}
 *										}
 *
 *	Using as above gives you confirmation that entire RX register was checked and there is no more pending data
 */

///////////////////////////////////////////////NRF24 TRANSMITTER'S COMMANDS///////////////////////////////////////////////////////////////////
void nrf24_send(unsigned char payload[], int length);
/*
 *	 this command is used to write payload to be send receiver via specified data pipe
 *                    		   max payload length is 32bytes
 *              length is length of payload. Using sizeof() is recommended
 *
 *       			 ex. uint8_t payload[14] = "this is payload";
 *            			ex. nrf24_send(payload, sizeof(payload);
 *
 *                             USE ONLY ON TRANSMITTER !!!!
 *
 */

void nrf24_read(unsigned char payload[], int length);
/*
 * this command reads acknowledgment payload which waits in transmitter's ACK payload register
 *
 * 						   			Usage with static arrays
 * ex. received_ACK[32] = ""; //always ensure that array is 32bytes and empty before reading
 * ex. nrf24_read(received_ACK, length); length is preset before reading operation with nrf24_payloadlength(); command
 *
 *                      Using nrf24_read command with dynamic arrays is impossible.
 * You can make received_data[32] array less than 32 bytes to save some memory unless you know exact ACK payload size
 *
 *                this command automatically resets status register so you don't have to do this manually
 */

int nrf24_ACKavailable();
/*
 * this command returns 1 if ACK has been received
 *
 * 							ex.
 *
 * if (nrf24_ACKavailable()){
 * 	//instruction you want to do when ACK has been received
 * }
 *
 * 				   USE ONLY ON TRANSMITTER !!!!
 */

int nrf24_payloadlength();
/*
 * this command returns length of ACK payload which is in transmitter's ACK payload register
 *
 * 								ex. int length;
 * 					  ex. length = nrf24_payloadlength();
 *
 * respectively for 1 byte ACK payload command returns 1; for 32 byte payload command returns 32
 *
 */

void nrf24_senduntilACK(unsigned char data[], int length);
/*
 * this commands keeps sending payload until it receives acknowledgment
 *   			it is used only with unstable connection
 * or when receiver starts listening periodically (ex. only 10sec each 1 minute)
 *
 *				 ex. uint8_t payload[7] = "payload";
 * 			ex. nrf24_senduntilACK(payload, sizeof(payload));
 *
 * 	   this command stops uC from doing next operations until nrf24 receive ACK
 * 		 	   (has the same effect on uC as delay function)
 */

/////////////////////////////////////////////////MULTIPURPOSE COMMANDS/////////////////////////////////////////////////////////////////////
void nrf24_resetstatus();
/*
 * 			this command resets status register of nrf24
 * use this command inside loop: if(nrf24_available()) or if(nrf24_ACKavailable())
 * 		if you don't read any payload or acknowledgment payload
 * also use this inside ack loop if you are using ACK without any payload
 *		  or any time you want to reset status register
 *
 * 		this function is implemented in nrf24_read() command
 */
void nrf24_flush();
/*
 * 		this command flushes both RX and TX registers on specified device
 */


/////////////////////////////////////////////ADVANCE CONFIG COMMANDS/////////////////////////////////////////////////////////////////////
//use this commands only if you exactly know what you are doing and yo have studied nrf24's datasheet and nrf24.c file
//always use this commands after nrf24_configRX() || nrf24_configTX() as config commands resets values to default
//setting some of those commands inappropriate may result in bricking NRF24 iC

/* 					 SPI_writereg(SETUP_RETR, value);
 *
 * use this command to change number of retransmits or delay between retransmits
 * 	    value should be in hex 0x00...0xFF according to wanted values
 *
 * ex. SPI_writereg(SETUP_RETR, 0x3F); //sets delay to 1000uS and up to 15 retransmits
 *
 *  this is implemented in config commands and set to 1000uS delay and 15 retransmits
 */
/*
 *					SPI_writereg(RF_CH, value);
 *
 * 	  use this command to change frequency which nrf24 operates on
 * 	  value should be in hex 0x00...0x7F according to wanted values
 *
 * ex. SPI_writereg(RF_CH, 0x02); //sets frequency value to 2400GHz + 0x02
 *
 * 			frequencies can be set between 2.4Ghz and 2.525Ghz
 * 			frequency formula FRQ = 2400 + RF_CH [MHz]
 *
 *  this is implemented in config commands and set to 2400Ghz + 0x02
 */
/*
 * 			  SPI_writereg(RF_SETUP, (speed_rate)|(power));
 * 	  use this command to change speed rate and output power of TX device
 *
 * 	  				TABLE OF SPEED RATES AND POWERS
 * 	  				      250kbps = 0x20;
 * 	  				      1Mbps   = 0x00;
 * 	  				      2Mbps   = 0x08;
 * 	  				      0dBm    = 0x06;
 * 	  				      -6dBm   = 0x04;
 * 	  				      -12dBm  = 0x02;
 * 	  				      -18dBm  = 0x00;
 *
 *	ex. SPI_writereg(RF_SETUP, (0x20)|(0x06);  //sets speed rate to 250kbps and power to 0dBm
 *
 *			using other values than given above may result in bricking nrf24 iC
 *
 *			this is implemented in config command and set to 2Mbps and 0dBm
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void nrf24_resetmax_rt();
int nrf24_senduntilACK1(unsigned char data[], int length);
void SPI_enable();
unsigned char SPI(char cData);
void SPI_writereg(unsigned char reg, unsigned char val);
unsigned char SPI_readreg(unsigned char reg);

volatile unsigned char SPI_memory;
volatile unsigned char W_buff;
volatile unsigned char R_buff;
volatile unsigned char value;
volatile unsigned char status;
volatile int reset;

#endif /* NRF24_H_ */
