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
 */

#include "nrf24.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

void SPI_enable(){
	SPCR |= (1<<SPE)|(1<<MSTR);
	SPCR |= (1<<SPR1);
}
void SPI_init(){
	MISO_reg &= ~(1<<MISO);
	MOSI_reg |= (1<<MOSI);
	CE_reg |= (1<<CE);
	CSN_reg |= (1<<CSN);
	SCK_reg |= (1<<SCK);
	SPI_enable();

	CSN_set |= (1<<CSN);
	CE_set &= ~(1<<CE);
}
unsigned char SPI(char cData){
	SPI_data_reg = cData;
	while(!(SPI_status_reg & (1<<SPI_interrupt_flag)));
	SPI_memory = SPI_data_reg;
	return SPI_data_reg;
}
void SPI_writereg(unsigned char reg, unsigned char val){
	CE_set &= ~(1<<CE);
	CSN_set &= ~(1<<CSN);
	W_buff = (W_REGISTER|(MASK_REGISTER & reg));
	SPI(W_buff);
	SPI(val);
	CSN_set |= (1<<CSN);
	_delay_us(1);
}
unsigned char SPI_readreg(unsigned char reg){
	CSN_set &= ~(1<<CSN);
	R_buff = (R_REGISTER|(MASK_REGISTER & reg));
	value = SPI(R_buff);
	value = SPI(0x00);
	CSN_set |= (1<<CSN);
	return value;
}
void nrf24_configTX(unsigned char address[]){
	_delay_ms(120);
	nrf24_pwrdown();
	CE_set &= ~(1<<CE);
	SPI_writereg(CONFIG, 0x0E);
	_delay_ms(2);
	SPI_writereg(EN_AA, 0x01);
	SPI_writereg(SETUP_AW, 0x03);
	SPI_writereg(SETUP_RETR, 0x3F);
	SPI_writereg(RF_CH, 0x02);
	SPI_writereg(RF_SETUP, 0x0E);
	SPI_writereg(STATUS, 0x70);

	CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P0)));
		for (int i=4; i>=0; i--){
			SPI(address[i]);
		}
	CSN_set |= (1<<CSN);

	CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & TX_ADDR)));
		for (int i=4; i>=0; i--){
			SPI(address[i]);
		}
	CSN_set |= (1<<CSN);

	SPI_writereg(DYNPD, (1<<1)|(1<<0));
	SPI_writereg(FEATURE, (1<<2)|(1<<1));
}

void nrf24_configRX(unsigned char address[]){
	_delay_ms(120);
	nrf24_pwrdown();
	CE_set &= ~(1<<CE);
	SPI_writereg(CONFIG, 0x0F);
	_delay_ms(2);
	SPI_writereg(EN_AA, 0x01);
	SPI_writereg(EN_RXADDR, 0x01);
	SPI_writereg(SETUP_AW, 0x03);
	SPI_writereg(SETUP_RETR, 0x3F);
	SPI_writereg(RF_CH, 0x02);
	SPI_writereg(RF_SETUP, 0x0E);
	SPI_writereg(STATUS, 0x70);

	CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P0)));
		for (int i=4; i>=0; i--){
			SPI(address[i]);
		}
	CSN_set |= (1<<CSN);

	SPI_writereg(DYNPD, (1<<1)|(1<<0));
	SPI_writereg(FEATURE, (1<<2)|(1<<1));
}
void nrf24_stoplistening(){
	CE_set &= ~(1<<CE);
	_delay_ms(10);
}
void nrf24_startlistening(){
	CE_set |= (1<<CE);
	_delay_ms(10);
}
void nrf24_pwrdown(){
	CSN_set &= ~(1<<CSN);
	SPI((W_REGISTER|(MASK_REGISTER & CONFIG)));
	SPI(0x0C);
	CSN_set |= (1<<CSN);
}
void nrf24_pwrupTX(){
	CSN_set &= ~(1<<CSN);
	SPI((W_REGISTER|(MASK_REGISTER & CONFIG)));
	SPI(0x0E);
	CSN_set |= (1<<CSN);
	_delay_ms(2);
}
void nrf24_pwrupRX(){
	CE_set &= ~(1<<CE);
	CSN_set &= ~(1<<CSN);
	SPI((W_REGISTER|(MASK_REGISTER & CONFIG)));
	SPI(0x0F);
	CSN_set |= (1<<CSN);
	_delay_ms(2);
}
void nrf24_send(unsigned char payload[], int length){
	CE_set &= ~(1<<CE);
	nrf24_pwrupTX();
	CSN_set &= ~(1<<CSN);
	SPI(FLUSH_TX);
	CSN_set |= (1<<CSN);
	CSN_set &= ~(1<<CSN);
	SPI(W_TX_PAYLOAD);
	for (int i=0; i<length; i++){
		SPI(payload[i]);
	}
	CSN_set |= (1<<CSN);
	CE_set |= (1<<CE);
	_delay_us(20);
	CE_set &= ~(1<<CE);
}
void nrf24_resetstatus(){
	SPI_writereg(STATUS, 0x70);
}
void nrf24_resetmax_rt(){
	SPI_writereg(STATUS, 0x10);
}
int nrf24_available(){
	status = SPI_readreg(STATUS);
	if ((status & 0x40) == 0x40){
		return 1;
	}
	else {
		return 0;
	}
}
void nrf24_read(unsigned char payload[], int length){
	CSN_set &= ~(1<<CSN);
	SPI(R_RX_PAYLOAD);
	for (int i=0; i<length; i++){
		payload[i] = SPI(0x00);
	}
	CSN_set |= (1<<CSN);
	nrf24_resetstatus();
	CSN_set &= ~(1<<CSN);
	SPI(FLUSH_RX);
	CSN_set |= (1<<CSN);
	nrf24_resetstatus();
}

int nrf24_ACKavailable(){
	status = SPI_readreg(STATUS);
	if ((status & 0x20) == 0x20){
		return 1;
	}
	else {
		return 0;
	}
}

void nrf24_writeACK(unsigned char ACK[], int pipe, int length){
	unsigned char pipe_bin;
	if (pipe == 0){
		pipe_bin = 0x00;
	}
	else if (pipe == 1){
		pipe_bin = 0x01;
	}
	else if (pipe == 2){
		pipe_bin = 0x02;
	}
	else if (pipe == 3){
		pipe_bin = 0x03;
	}
	else if (pipe == 4){
		pipe_bin = 0x04;
	}
	else if (pipe == 5){
		pipe_bin = 0x05;
	}
	else {
		pipe_bin = 0x00;
	}
	CSN_set &= ~(1<<CSN);
	SPI(W_ACK_PAYLOAD | pipe_bin);
	for (int i=0; i<length; i++){
		SPI(ACK[i]);
	}
	CSN_set |= (1<<CSN);
}

int nrf24_payloadlength(){
	int length;
	CSN_set &= ~(1<<CSN);
	SPI(R_RX_PL_WID);
	length = SPI(0x00);
	CSN_set |= (1<<CSN);
	return length;
}

int nrf24_senduntilACK1(unsigned char data[], int length){
	status = SPI_readreg(STATUS);
	if ((status & 0x10) == 0x10){
		nrf24_resetmax_rt();
	}
	nrf24_send(data, length);
	_delay_ms(100);
	if (nrf24_ACKavailable()){
		return 0;
	}
	else {
		return 1;
	}
}

void nrf24_senduntilACK(unsigned char data[], int length){
	while (nrf24_senduntilACK1(data, length));
}

void nrf24_TXaddr(unsigned char address[]){
	CE_set &= ~(1<<CE);
	CSN_set &= ~(1<<CSN);
	SPI((W_REGISTER|(MASK_REGISTER & TX_ADDR)));
	for (int i=4; i>=0; i--){
		SPI(address[i]);
	}
	CSN_set |= (1<<CSN);
	__asm__ __volatile__("nop");
	CSN_set &= ~(1<<CSN);
	SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P0)));
	for (int i=4; i>=0; i--){
		SPI(address[i]);
	}
	CSN_set |= (1<<CSN);
}
void nrf24_RXaddr(unsigned char address[], int pipe){
	CE_set &= ~(1<<CE);
	if (pipe == 0){
		CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P0)));
		for (int i=4; i>=0; i--){
			SPI(address[i]);
		}
		CSN_set |= (1<<CSN);
		return;
	}
	else if (pipe == 1){
		SPI_writereg(EN_AA, (1<<1)|(1<<0));
		SPI_writereg(EN_RXADDR, (1<<1)|(1<<0));
		CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P1)));
		for (int i=4; i>=0; i--){
			SPI(address[i]);
		}
		CSN_set |= (1<<CSN);
		SPI_writereg(DYNPD, (1<<1)|(1<<0));
		return;
	}
	else if (pipe == 2){
		SPI_writereg(EN_AA, (1<<2)|(1<<1)|(1<<0));
		SPI_writereg(EN_RXADDR, (1<<2)|(1<<1)|(1<<0));
		CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P2)));
		SPI(address[4]);
		CSN_set |= (1<<CSN);
		SPI_writereg(DYNPD, (1<<2)|(1<<1)|(1<<0));
		return;
	}
	else if (pipe == 3){
		SPI_writereg(EN_AA, (1<<3)|(1<<2)|(1<<1)|(1<<0));
		SPI_writereg(EN_RXADDR, (1<<3)|(1<<2)|(1<<1)|(1<<0));
		CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P3)));
		SPI(address[4]);
		CSN_set |= (1<<CSN);
		SPI_writereg(DYNPD, (1<<3)|(1<<2)|(1<<1)|(1<<0));
		return;
	}
	else if (pipe == 4){
		SPI_writereg(EN_AA, (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
		SPI_writereg(EN_RXADDR, (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
		CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P4)));
		SPI(address[4]);
		CSN_set |= (1<<CSN);
		SPI_writereg(DYNPD, (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
		return;
	}
	else if (pipe == 5){
		SPI_writereg(EN_AA, (1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
		SPI_writereg(EN_RXADDR, (1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
		CSN_set &= ~(1<<CSN);
		SPI((W_REGISTER|(MASK_REGISTER & RX_ADDR_P5)));
		SPI(address[4]);
		CSN_set |= (1<<CSN);
		SPI_writereg(DYNPD, (1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0));
		return;
	}
	else {
		return;
	}
}

int nrf24_pipenum(){
	status = SPI_readreg(STATUS);
	if ((status & 0x02) == 0x02){
		return 1;
	}
	else if ((status & 0x04) == 0x04){
		return 2;
	}
	else if ((status & 0x06) == 0x06){
		return 3;
	}
	else if ((status & 0x08) == 0x08){
		return 4;
	}
	else if ((status & 0x0A) == 0x0A){
		return 5;
	}
	else if ((status & 0x00) == 0x00){
		return 0;
	}
	else {
		return 6;
	}
}

void nrf24_flush(){
	CSN_set &= ~(1<<CSN);
	SPI(FLUSH_TX);
	CSN_set |= (1<<CSN);
	CSN_set &= ~(1<<CSN);
	SPI(FLUSH_RX);
	CSN_set |= (1<<CSN);
}

void nrf24_readmulticeiver(unsigned char payload[], int length){
	CSN_set &= ~(1<<CSN);
	SPI(R_RX_PAYLOAD);
	for (int i=0; i<length; i++){
		payload[i] = SPI(0x00);
	}
	CSN_set |= (1<<CSN);
	status = SPI_readreg(STATUS);
	if ((status & 0x0E) == 0x0E){
		nrf24_resetstatus();
	}
}
