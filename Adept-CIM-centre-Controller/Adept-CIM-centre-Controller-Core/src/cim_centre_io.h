/*
* cim_centre_io.h
*
* Created: 29/11/2020 9:20:57 AM
*  Author: jbonwick
*/


#ifndef CIM_CENTRE_IO_H_
#define CIM_CENTRE_IO_H_

#include "asf.h"

/* ----------- Pins ---------- */

//Data bus
#define DATA_BUS_MASK 0x000001FE
#define PDBD0 IOPORT_CREATE_PIN(PIOC, 1)   // DUE_PIN = 33
#define PDBD1 IOPORT_CREATE_PIN(PIOC, 2)   // DUE_PIN = 34
#define PDBD2 IOPORT_CREATE_PIN(PIOC, 3)   // DUE_PIN = 35
#define PDBD3 IOPORT_CREATE_PIN(PIOC, 4)   // DUE_PIN = 36
#define PDBD4 IOPORT_CREATE_PIN(PIOC, 5)   // DUE_PIN = 37
#define PDBD5 IOPORT_CREATE_PIN(PIOC, 6)   // DUE_PIN = 38
#define PDBD6 IOPORT_CREATE_PIN(PIOC, 7)   // DUE_PIN = 39
#define PDBD7 IOPORT_CREATE_PIN(PIOC, 8)   // DUE_PIN = 40
//Control bus
#define PBNIOR IOPORT_CREATE_PIN(PIOC, 9)  // DUE_PIN = 41
#define PBNIOW IOPORT_CREATE_PIN(PIOC, 12) // DUE_PIN = 51
//Address bus
#define ADDRESS_BUS_MASK 0x000FE000
#define PBA0 IOPORT_CREATE_PIN(PIOC, 13)   // DUE_PIN = 50
#define PBA1 IOPORT_CREATE_PIN(PIOC, 14)   // DUE_PIN = 49
#define PBA2 IOPORT_CREATE_PIN(PIOC, 15)   // DUE_PIN = 48
#define PBA3 IOPORT_CREATE_PIN(PIOC, 16)   // DUE_PIN = 47
#define PBA4 IOPORT_CREATE_PIN(PIOC, 17)   // DUE_PIN = 46
#define PBA5 IOPORT_CREATE_PIN(PIOC, 18)   // DUE_PIN = 45
#define PBA6 IOPORT_CREATE_PIN(PIOC, 19)   // DUE_PIN = 44
//More control lines
#define PBRES IOPORT_CREATE_PIN(PIOC, 21)   // DUE_PIN = 9
#define PBAEN IOPORT_CREATE_PIN(PIOC, 22)   // DUE_PIN = 8
#define PDDS IOPORT_CREATE_PIN(PIOC, 23)    // DUE_PIN = 7
#define PMSEL IOPORT_CREATE_PIN(PIOC, 24)   // DUE_PIN = 6
#define PFAULT1 IOPORT_CREATE_PIN(PIOC, 25) // DUE_PIN = 5
#define PFAULT2 IOPORT_CREATE_PIN(PIOC, 26) // DUE_PIN = 4
#define PHOOD IOPORT_CREATE_PIN(PIOC, 28)   // DUE_PIN = 3
#define PESTOPL IOPORT_CREATE_PIN(PIOC, 29) // DUE_PIN = 10

/* ----------- Chip Addresses---------- */
#define FRONT_PANEL_ADDRESS 0x5C

#define PIO0_ADDRESS 0x10
#define PIO1_ADDRESS 0x11
#define PIO2_ADDRESS 0x12
#define PIO_COMMAND_ADDRESS 0x13

#define IO_ADDRESS 0x58
#define LOAD_DAC_ADDRESS 0x26
//Spindle
//#define SPINDLE_ENCODER_MSB_ADDRESS 0x28
//#define SPINDLE_ENCODER_LSB_ADDRESS 0x29
//#define SPINDLE_ENCODER_RESET_ADDRESS 0x40
// #define SPINDLE_DAC_LSB_ADDRESS 0x14
// #define SPINDLE_DAC_MSB_ADDRESS 0x18
//X-Axis
//#define X_ENCODER_ADDRESS 0x30
//#define X_ENCODER_RESET_ADDRESS 0x48
//#define X_DAC_LSB_ADDRESS 0x15
//#define X_DAC_MSB_ADDRESS 0x19
//Y-Axis
//#define Y_ENCODER_ADDRESS 0x2C
//#define Y_ENCODER_RESET_ADDRESS 0x44
//#define Y_DAC_LSB_ADDRESS 0x17
//#define Y_DAC_MSB_ADDRESS 0x1B
//Z-Axis
//#define Z_ENCODER_ADDRESS 0x34
//#define Z_ENCODER_RESET_ADDRESS 0x4C
// #define Z_DAC_LSB_ADDRESS 0x16
// #define Z_DAC_MSB_ADDRESS 0x1A

typedef enum {
	forwardBackward = 0x30,
	leftRight = 0x2C,
	upDown = 0x34,
	spindle = 0x28
}encoder_t;

typedef enum {
	forwardBackward = 0x48,
	leftRight = 0x44,
	upDown= 0x4C,
	spindle = 0x40
}encoderReset_t;

typedef enum {
	forwardBackward = 0x15,
	leftRight = 0x17,
	upDown = 0x16,
	spindle = 0x14
}dac_t;

struct fPanelStatus_t {
	unsigned int zMinus:1;
	unsigned int zPlus:1;
	unsigned int xMinus: 1;
	unsigned int xPlus: 1;
	unsigned int home: 1;
	unsigned int exit: 1;
	unsigned int rapid: 1;
	unsigned int sPlus: 1;
	unsigned int feed: 1;
	unsigned int stop: 1;
	unsigned int sMinus: 1;
	unsigned int slow: 1;
};


/* Sets up the pins on the bus */
void io_init(void);

void writeRaw(uint8_t address, uint8_t data);

uint8_t readRaw(uint8_t address);

/* Returns a 16 bit value from the encoder counter at the given address*/
uint16_t readEncoder(encoder_t);

/* Reset the encoder counter at the given address to 0 */
void resetEncoder(encoderReset_t);

/* Set the dac to the provided speed */
void setDac(dac_t, uint16_t speed);

void loadDacs(void);






/* Reads the 12 buttons on the front panel,
* each bit represents a button where 0 is pressed.
* | 0 = Z- | 1 = Z+ | 2 = x- | 3 = x+ |
* | 4 = HOME | 5 = EXIT | 6 = RAPID | 7 = S+ |
* | 8 = FEED | 9 = STOP | 10 = S- | 11 = SLOW |
*
*/
uint16_t readFrontPanel(void);

/* Test IO function */
uint16_t readIO(void);

/* Pass a 12 value to set the dacS. All DACs must be set at the same time.
* If a DAC doesn't need changing pass 0xFF for axis instead.
*/
void setDacs(uint16_t xVoltage, uint16_t yVoltage, uint16_t zVoltage, uint16_t sVoltage);

#endif /* CIM_CENTRE_IO_H_ */