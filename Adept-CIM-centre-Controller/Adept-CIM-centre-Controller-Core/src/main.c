/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# "Insert system clock initialization code here" comment
* -# Minimal main function that starts with a call to board_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/
#include <asf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cim_centre_io.h"
#include "console.h"
#include "pid.h"

#define AXIS_HOMED 0 // Axis has been homed
#define AXIS_NEEDS_HOMING 1 //Axis has not been homed yet
#define AXIS_HOMING_ENDSTOP 2 //When homing towards the end stop
#define AXIS_HOMING_INDEX 3 //When homing towards encoder index pulse

#define FB_MAX 198000

void updateEncoder(int *pos, uint16_t encoderCounter);

void configure_rtt(void){
	rtt_init(RTT, 30);
	rtt_write_alarm_time(RTT, 5);
}

int main (void)
{
	sysclk_init();
	board_init();

	configure_console();
	
	//Initialize io pins on the bus
	io_init();
	
	//Set axis positions to the current pos so that the rollover logic doesn't trigger a roll over
	int fb_current = readEncoder(encoderFB);
	int lr_current = readEncoder(encoderLR);
	int ud_curent = readEncoder(encoderUD);
	//Set target positions to the current position
	int fb_target = fb_current;
	int lr_target = lr_current;
	int ud_target = ud_curent;
	//Current Axis speed
	int fb_speed = 0;
	int lr_speed = 0;
	
	uint16_t frontPanelStatus = readFrontPanel();
	uint16_t generalIO = readIO();
	
	//Set to true if currently homing
	uint8_t FBhomeStatus = 0;
	uint8_t LRhomeStatus = 0;
	
	//PID structs
	struct pid_t fb_pid;
	struct pid_t lr_pid;
	//Configure the PID structs
	pidConfigure(&fb_pid, &fb_current, &fb_speed, &fb_target, 0.5, 10, 2, -2024, 2024);
	pidConfigure(&lr_pid, &lr_current, &lr_speed, &lr_target, 0.5, 3, 2, -2024, 2024);
	
	configure_rtt();
	bool hadAlarm = false;
	int fb_step_state = 0;
	int lr_step_state = 0;

	while(1){
		//Check for RTT alarm
		if((rtt_get_status(RTT) & RTT_SR_ALMS) == RTT_SR_ALMS){
			hadAlarm = true;
			//Reset alarm
			configure_rtt();
		}
		
		//Read quadrature encoders
		updateEncoder(&fb_current, readEncoder(encoderFB));
		updateEncoder(&lr_current, readEncoder(encoderLR));
		updateEncoder(&ud_curent, readEncoder(encoderUD));
		
		//Calculate PID if alarm has occurred in the RTT
		if(hadAlarm){
			//limit FB
			if(fb_target > FB_MAX){
				fb_target = FB_MAX;
			}
			//Get output values for setting the DACs
			pidCalculate(&fb_pid);
			pidCalculate(&lr_pid);
			
			//Add 2047 to center the values around 2048 as zero speed
			setDacs(-fb_speed + 2048, 2047,-lr_speed + 2048, 2048);
		}
		
		frontPanelStatus = readFrontPanel();
		generalIO = readIO();
		
		
		//if(frontPanelStatus & FP_X_PLUS){
			//setDacs(4095, 2048, 2048, 2048);
		//}
		//else if(frontPanelStatus & FP_X_MINUS){
			//setDacs(0, 2048, 2048, 2048);
		//}
		//else if(frontPanelStatus & FP_Z_MINUS){
			//setDacs(2048, 2048, 4095, 2048);
		//}
		//else if(frontPanelStatus & FP_Z_PLUS){
			//setDacs(2048, 2048, 0, 2048);
		//}
		//else {
			//setDacs(2048, 2048, 2048, 2048);
		//}
		
		//Check for homing
		if(frontPanelStatus & FP_HOME){
			if(frontPanelStatus & (FP_X_MINUS | FP_X_PLUS)){
				FBhomeStatus = AXIS_HOMING_ENDSTOP;
			}
			else if(frontPanelStatus & (FP_Z_MINUS | FP_Z_PLUS)){
				LRhomeStatus = AXIS_HOMING_ENDSTOP;
			}
		}
		//Check for movement buttons every 5ms
		else if(hadAlarm){
			if(frontPanelStatus & FP_Z_PLUS){
				lr_target += 200;
			}
			if(frontPanelStatus & FP_Z_MINUS){
				lr_target -= 200;
			}
			if(frontPanelStatus & FP_X_PLUS){
				fb_target -= 200;
				if(fb_target < 0){
					fb_target = 0;
				}
			}
			if(frontPanelStatus & FP_X_MINUS){
				fb_target += 200;
			}
		}
		
		//Homing logic
		if(FBhomeStatus == AXIS_HOMING_ENDSTOP){
			if(generalIO & FB_ENDSTOP){
				FBhomeStatus = AXIS_HOMING_INDEX;
			}
			else if(hadAlarm){
				fb_target -= 80;
			}
		}
		else if(FBhomeStatus == AXIS_HOMING_INDEX){
			//Check if index has occurred while not in endstop region
			if((generalIO & FB_INDEX) && !(generalIO & FB_ENDSTOP)){
				FBhomeStatus = AXIS_HOMED;
				resetEncoder(encoderResetFB);
				fb_current = 0;
				fb_target = 0;
				updateEncoder(&fb_current, readEncoder(encoderFB));
			}
			else if(hadAlarm){
				fb_target += 1;
			}
		}
		
		if(LRhomeStatus == AXIS_HOMING_ENDSTOP){
			if(generalIO & LR_ENDSTOP){
				LRhomeStatus = AXIS_HOMING_INDEX;
			}
			else if(hadAlarm){
				lr_target += 80;
			}
		}
		else if(LRhomeStatus == AXIS_HOMING_INDEX){
			//Check if index has occurred while not in endstop region
			if((generalIO & LR_INDEX) && !(generalIO & LR_ENDSTOP)){
				LRhomeStatus = AXIS_HOMED;
				resetEncoder(encoderResetLR);
				lr_current = 0;
				lr_target = 0;
				updateEncoder(&lr_current, readEncoder(encoderLR));
			}
			else if(hadAlarm){
				lr_target -= 1;
			}
		}
		
		if(hadAlarm){
		//Print some useful info to the console
		char consoleBuffer[100];
		//usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "FB: %d\r\n", fb_current);
		usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "FBT: %d\r\n", fb_target);
		usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "LR: %d\r\n", lr_current);
		usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "LRT: %d\r\n", lr_target);
		usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "UD: %d\r\n", ud_curent);
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "S: %d\r\n", readEncoder(encoderSpindle));
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "FP: %d\r\n", readFrontPanel());
		//usart_write_line(CONF_UART, consoleBuffer);
		sprintf(consoleBuffer, "P0: %d\r\n", readIO());
		usart_write_line(CONF_UART, consoleBuffer);
		}
		
		if(hadAlarm){
			hadAlarm = false;
		}
		
		int temp_state = ioport_get_pin_level(PFBSTEP);
		if((temp_state == 1) && (fb_step_state == 0)){
			if (ioport_get_pin_level(PFBDIR))
			{
				fb_target++;
			}
			else{
				fb_target--;
			}
		}
		fb_step_state = temp_state;
		
		temp_state = ioport_get_pin_level(PLRSTEP);
		if((temp_state == 1) && (lr_step_state == 0)){
			if (ioport_get_pin_level(PLRDIR))
			{
				lr_target++;
			}
			else{
				lr_target--;
			}
		}
		lr_step_state = temp_state;
	}
}

void updateEncoder(int *pos, uint16_t encoderCounter){
	//Check for rollover
	if(abs((*pos & 0xFFFF) - encoderCounter) > 30000){
		//Check if roll down
		int temp = *pos >> 16;
		if(encoderCounter & 0x8000){
			temp--;
		}
		else{
			temp++;
		}
		*pos = temp << 16;
		*pos |= encoderCounter;
	}
	else{
		*pos &= 0xFFFF0000;
		*pos |= encoderCounter;
	}
}

