/*
* cim_centre_io.c
*
* Created: 29/11/2020 9:20:45 AM
*  Author: jbonwick
*/

#include "cim_centre_io.h"

void io_init(void) {
	//Configure Address ports
	ioport_set_port_dir(IOPORT_PIOC, ADDRESS_BUS_MASK, IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PIOC, ADDRESS_BUS_MASK, 0);
	
	//Configure data lines
	ioport_set_port_dir(IOPORT_PIOC, DATA_BUS_MASK, IOPORT_DIR_INPUT);
	
	//Configure control lines
	ioport_set_pin_dir(PBNIOR, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PBNIOR, 1);//Pull low to read
	
	ioport_set_pin_dir(PBNIOW, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PBNIOW, 1);//Pull low to write
	
	ioport_set_pin_dir(PBRES, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PBRES, 0); //Pull high to reset
	
	ioport_set_pin_dir(PBAEN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PBAEN, 1); //Pull high to enable
	
	//Pin use currently unknown
	ioport_set_pin_dir(PDDS, IOPORT_DIR_INPUT);
	//ioport_set_pin_level(PDDS, 1);
	
	ioport_set_pin_dir(PMSEL, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PMSEL, 1); //Pull low to enable
	
	//UNKNOWN????
	ioport_set_pin_dir(PFAULT1, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(PFAULT2, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(PHOOD, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(PESTOPL, IOPORT_DIR_INPUT);
	
	//Reset everything on the bus
	ioport_set_pin_level(PBRES, 1);
	delay_ms(10);
	ioport_set_pin_level(PBRES, 0);
	
	//Set up the PIO chip with all three ports set to outputs
	writeRaw(PIO_COMMAND_ADDRESS, 0x80);
	
	writeRaw(PIO0_ADDRESS, 0x10);
	writeRaw(PIO1_ADDRESS, 0x00);
	writeRaw(PIO2_ADDRESS, 0x00);
}

void writeRaw(uint8_t address, uint8_t data) {
	//Clear the address
	ioport_set_port_level(IOPORT_PIOC, ADDRESS_BUS_MASK, 0);
	//Set the address
	ioport_set_port_level(IOPORT_PIOC, ADDRESS_BUS_MASK & (address << 13), 1);
	
	ioport_set_port_dir(IOPORT_PIOC, DATA_BUS_MASK, IOPORT_DIR_OUTPUT);
	ioport_set_port_level(IOPORT_PIOC, DATA_BUS_MASK, 0);
	ioport_set_port_level(IOPORT_PIOC, DATA_BUS_MASK & (data << 1), 1);
	//Set MSEL low
	ioport_set_pin_level(PMSEL, 0);
	
	ioport_set_pin_level(PBNIOW, 0);
	// 210 ns delay
	for (int i = 0; i <= 14; i++)
	{
		__asm__ __volatile__ ("nop");
	}
	
	ioport_set_pin_level(PMSEL, 1);
	
	ioport_set_pin_level(PBNIOW, 1);
	
	ioport_set_port_dir(IOPORT_PIOC, DATA_BUS_MASK, IOPORT_DIR_INPUT);
}

uint8_t readRaw(uint8_t address) {
	//Clear the address
	ioport_set_port_level(IOPORT_PIOC, ADDRESS_BUS_MASK, 0);
	//Set the address
	ioport_set_port_level(IOPORT_PIOC, ADDRESS_BUS_MASK & (address << 13), 1);
	//Set MSEL low
	ioport_set_pin_level(PMSEL, 0);
	//Put read low to initiate a read
	ioport_set_pin_level(PBNIOR, 0);
	//!!! Must wait 200ns before read. The NOP instruction takes ~14ns to execute.
	for (int i = 0; i <= 14; i++)
	{
		__asm__ __volatile__ ("nop");
	}
	//Read MSB
	uint8_t data = ioport_get_port_level(IOPORT_PIOC, DATA_BUS_MASK) << 7;
	//Put the read pin high
	ioport_set_pin_level(PBNIOR, 1);
	ioport_set_pin_level(PMSEL, 1);
	return data;
}

uint16_t readEncoder(encoder_t encoder) {
	//Read MSB
	uint16_t data = readRaw(encoder) << 8;
	//Read LSB
	data |= readRaw(encoder + 1);
	return data;
}

void resetEncoder(encoderReset_t encoder) {
	readRaw(encoder);
}

void setDac(dac_t dac, uint16_t speed){
	//Set LSB
	writeRaw(dac, speed);
	//Set MSB
	writeRaw(dac + 4, speed >> 8);
}

void loadDacs(void){
	writeRaw(LOAD_DAC_ADDRESS);
}