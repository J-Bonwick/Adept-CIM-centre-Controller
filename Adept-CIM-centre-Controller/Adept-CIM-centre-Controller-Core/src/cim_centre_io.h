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

// Data bus
#define DATA_BUS_MASK 0x000001FE
#define PDBD0 IOPORT_CREATE_PIN(PIOC, 1) // DUE_PIN = 33
#define PDBD1 IOPORT_CREATE_PIN(PIOC, 2) // DUE_PIN = 34
#define PDBD2 IOPORT_CREATE_PIN(PIOC, 3) // DUE_PIN = 35
#define PDBD3 IOPORT_CREATE_PIN(PIOC, 4) // DUE_PIN = 36
#define PDBD4 IOPORT_CREATE_PIN(PIOC, 5) // DUE_PIN = 37
#define PDBD5 IOPORT_CREATE_PIN(PIOC, 6) // DUE_PIN = 38
#define PDBD6 IOPORT_CREATE_PIN(PIOC, 7) // DUE_PIN = 39
#define PDBD7 IOPORT_CREATE_PIN(PIOC, 8) // DUE_PIN = 40
// Control bus
#define PBNIOR IOPORT_CREATE_PIN(PIOC, 9)  // DUE_PIN = 41
#define PBNIOW IOPORT_CREATE_PIN(PIOC, 12) // DUE_PIN = 51
// Address bus
#define ADDRESS_BUS_MASK 0x000FE000
#define PBA0 IOPORT_CREATE_PIN(PIOC, 13) // DUE_PIN = 50
#define PBA1 IOPORT_CREATE_PIN(PIOC, 14) // DUE_PIN = 49
#define PBA2 IOPORT_CREATE_PIN(PIOC, 15) // DUE_PIN = 48
#define PBA3 IOPORT_CREATE_PIN(PIOC, 16) // DUE_PIN = 47
#define PBA4 IOPORT_CREATE_PIN(PIOC, 17) // DUE_PIN = 46
#define PBA5 IOPORT_CREATE_PIN(PIOC, 18) // DUE_PIN = 45
#define PBA6 IOPORT_CREATE_PIN(PIOC, 19) // DUE_PIN = 44
// More control lines
#define PBRES IOPORT_CREATE_PIN(PIOC, 21)   // DUE_PIN = 9  | RESET
#define PBAEN IOPORT_CREATE_PIN(PIOC, 22)   // DUE_PIN = 8  |
#define PDDS IOPORT_CREATE_PIN(PIOC, 23)    // DUE_PIN = 7
#define PMSEL IOPORT_CREATE_PIN(PIOC, 24)   // DUE_PIN = 6
#define PFAULT1 IOPORT_CREATE_PIN(PIOC, 25) // DUE_PIN = 5
#define PFAULT2 IOPORT_CREATE_PIN(PIOC, 26) // DUE_PIN = 4
#define PHOOD IOPORT_CREATE_PIN(PIOC, 28)   // DUE_PIN = 3
#define PESTOPL IOPORT_CREATE_PIN(PIOC, 29) // DUE_PIN = 10

#define PFBSTEP IOPORT_CREATE_PIN(PIOD, 7)
#define PFBDIR IOPORT_CREATE_PIN(PIOD, 8)
#define PLRSTEP IOPORT_CREATE_PIN(PIOA, 16)
#define PLRDIR IOPORT_CREATE_PIN(PIOA, 24)

/* ----------- Chip Addresses---------- */
#define FRONT_PANEL_ADDRESS 0x5C

#define PIO0_ADDRESS 0x10
#define PIO1_ADDRESS 0x11
#define PIO2_ADDRESS 0x12
#define PIO_COMMAND_ADDRESS 0x13

#define IO_ADDRESS 0x58
#define LOAD_DAC_ADDRESS 0x26
// Spindle
//#define SPINDLE_ENCODER_MSB_ADDRESS 0x28
//#define SPINDLE_ENCODER_LSB_ADDRESS 0x29
//#define SPINDLE_ENCODER_RESET_ADDRESS 0x40
#define SPINDLE_DAC_LSB_ADDRESS 0x14
#define SPINDLE_DAC_MSB_ADDRESS 0x18

// FB-Axis
//#define X_ENCODER_ADDRESS 0x30
//#define X_ENCODER_RESET_ADDRESS 0x48
#define FB_DAC_LSB_ADDRESS 0x15
#define FB_DAC_MSB_ADDRESS 0x19

// LR-Axis
//#define Z_ENCODER_ADDRESS 0x34
//#define Z_ENCODER_RESET_ADDRESS 0x4C
#define LR_DAC_LSB_ADDRESS 0x16
#define LR_DAC_MSB_ADDRESS 0x1A

// Y-Axis
//#define Y_ENCODER_ADDRESS 0x2C
//#define Y_ENCODER_RESET_ADDRESS 0x44
#define Y_DAC_LSB_ADDRESS 0x17
#define Y_DAC_MSB_ADDRESS 0x1B

/* Below values are for LSB, MSB is LSB addr + 4 */
typedef enum {
  DAC_FB = 0x17,
  DAC_LR = 0x15,
  DAC_UD = 0x16,
  DAC_SPINDLE = 0x14,
} dac_t;

typedef enum {
  ENCODER_FB = 0x30,
  ENCODER_LR = 0x34,
  ENCODER_UD = 0x2C,
  ENCODER_SPINDLE_LATHE = 0x28,
  ENCODER_SPINDLE_MILL = 0x28, /* TODO: Wrong address */
} encoder_t;

typedef enum {
  ENCODER_RESET_FB = 0x48,
  ENCODER_RESET_LR = 0x4C,
  ENCODER_RESETUD = 0x44,
  ENCODER_RESET_SPINDLE_LATHE = 0x40,
  ENCODER_RESET_SPINDLE_MILL = 0x40, /* TODO: Wrong addrress */
} encoder_reset_t;

// Front panel - Use this with the data returned from readFrontPanel();
/* TODO: Do these change on mode change */
#define FP_Z_MINUS 0x1
#define FP_Z_PLUS 0x2
#define FP_X_MINUS 0x4
#define FP_X_PLUS 0x8
#define FP_HOME 0x10
#define FP_EXIT 0x20
#define FP_RAPID 0x40
#define FP_S_PLUS 0x80
#define FP_FEED 0x100
#define FP_STOP 0x200
#define FP_S_MINUS 0x400
#define FP_SLOW 0x800

// End Stops
#define W_ENDSTOP (1U << 0)
#define FB_ENDSTOP (1U << 1)
#define UD_ENDSTOP (1U << 2)
#define LR_ENDSTOP (1U << 3)
// E-Stop
#define ESTOP_LOW (1U << 4)
#define ESTOP_HIGH (1U << 5)
// Index
#define S_INDEX (1U << 6)
#define FB_INDEX (1U << 7)
#define UD_INDEX (1U << 8) //??? swap these?
#define LR_INDEX (1U << 9) //???

// ENJD_W = 1 ???
// END_FB = 2
// END_UD = 3 ???
// END_LR = 4
// Stop = 5-6 / 13/14
// Spindle = 7th / 15th
// FB index - 8th / 16 bit bit
// LR index = 9th bit
// UD index = 10th bit
// W Index = 11 ???
// V index = 12???

/* Sets up the pins on the bus */
void io_init(void);
/************************************************************************/
/*                                                                      */
/************************************************************************/
void writeRaw(uint8_t address, uint8_t data);

uint8_t readRaw(uint8_t address);

/* Returns a 16 bit value from the encoder counter at the given address*/
uint16_t readEncoder(encoder_t counter);

/* Reset the encoder counter at the given address to 0 */
void resetEncoder(encoderReset_t);

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

/* TODO Test me, may not work
 * dac value is a signed 12 bit value */
void dac_set(enum dac_t dac_addr, uint16_t dac_voltage);

/* Pass a 12 bit value to set the dacS. All DACs must be set at the same time.
 * If a DAC doesn't need changing pass 0xFF for axis instead.
 */
void set_dacs(uint16_t fb_voltage, uint16_t lr_voltage, uint16_t ud_oltage,
              uint16_t spindle_voltage);

#endif /* CIM_CENTRE_IO_H_ */
