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

void updateEncoder(long int *pos, uint16_t encoderCounter);

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
	long int fb_current = readEncoder(encoderFB);
	long int lr_current = readEncoder(encoderLR);
	long int ud_curent = readEncoder(encoderUD);
	//Set target positions to the current position
	long int fb_target = fb_current;
	long int lr_target = lr_current;
	long int ud_target = ud_curent;
	//Current Axis speed
	int fb_speed = 0;
	int lr_speed = 0;
	
	uint16_t frontPanelStatus = readFrontPanel();
	uint16_t generalIO = readIO();
	
	//Set to true if currently homing
	bool FBhoming = false;
	bool LRhoming = false;
	
	//PID structs
	pid_t fb_pid;
	pid_t lr_pid;
	//Configure the PID structs
	pidConfigure(&fb_pid, &fb_current, &fb_speed, &fb_target, 1, 0, 0, -2024, 2024);
	pidConfigure(&lr_pid, &lr_current, &lr_speed, &lr_target, 0.5, 0, 0, -2024, 2024);
	
	//resetEncoder(encoderResetFB);
	//resetEncoder(encoderResetLR);
	//resetEncoder(encoderResetSpindle);
	
	configure_rtt();

	while(1){
		//Read quadrature encoders
		updateEncoder(&fb_current, readEncoder(encoderFB));
		updateEncoder(&lr_current, readEncoder(encoderLR));
		updateEncoder(&ud_curent, readEncoder(encoderUD));
		
		//Calculate PID if alarm has occurred in the RTT
		if((rtt_get_status(RTT) & RTT_SR_ALMS) == RTT_SR_ALMS){
			//Get out put values for setting the DACs
			pidCalculate(fb_pid);
			pidCalculate(lr_pid);
			
			//Add 2047 to center the values around 2048 as zero speed
			setDacs(fb_speed + 2047, lr_speed + 2047, 2048, 2048);
			
			//long int lr_error = lr_pos - lr_target;
			//int lr_speed = lr_error * 0.5;
			//if(lr_speed > 2047){
				//lr_speed = 2047;
			//}
			//else if(lr_speed < -2047){
				//lr_speed = -2047;
			//}
			//
			//long int fb_error = fb_pos - fb_target;
			//int fb_speed = fb_error * 1;
			//if(fb_speed > 2047){
				//fb_speed = 2047;
			//}
			//else if(fb_speed < -2047){
				//fb_speed = -2047;
			//}
			
			//setDacs(fb_speed + 2048, 2048,lr_speed + 2048, 2048);
			//Reset the timer counter
			//configure_rtt();
		}
		
		frontPanelStatus = readFrontPanel();
		generalIO = readIO();
		
		
		//home logic
		if(FBhoming){
			if(generalIO & FB_ENDSTOP){
				FBhoming = false;
				//resetEncoder(encoderResetFB);
			}
			fb_target -= 40;
		}
		if(LRhoming){
			if(generalIO & LR_ENDSTOP){
				LRhoming = false;
				//resetEncoder(encoderResetLR);
			}
			lr_target += 40;
		}
		
		if(frontPanelStatus & FP_HOME){
			if(frontPanelStatus & FP_Z_PLUS){
				LRhoming = true;
			}
			if(frontPanelStatus & FP_X_PLUS){
				FBhoming = true;
			}
		}
		
		//switch (frontPanelStatus){
			//case FP_X_PLUS:
				//fb_target -= 40;
				//break;
			//case FP_X_MINUS:
				//fb_target += 40;
				//break;
			//case FP_Z_PLUS:
				//lr_target -= 40;
				//break;
			//case FP_Z_MINUS:
				//lr_target += 40;
			//default:
				//
			//
		//}
		
		if(frontPanelStatus & FP_X_PLUS){
			setDacs(4095, 2048, 2045, 2048);
		}
		else if(frontPanelStatus & FP_X_MINUS){
			setDacs(0, 2048, 2045, 2048);
		}
		else if(frontPanelStatus & FP_Z_MINUS){
			setDacs(2042, 2048, 4095, 2048);
		}
		else if(frontPanelStatus & FP_Z_PLUS){
			setDacs(2042, 2048, 0, 2048);
		}
		else {
			setDacs(2042, 2048, 2045, 2048);
		}
		if(frontPanelStatus & FP_Z_PLUS){
			lr_target += 40;
		}
		if(frontPanelStatus & FP_Z_MINUS){
			lr_target -= 40;
		}
		
		
		if((rtt_get_status(RTT) & RTT_SR_ALMS) == RTT_SR_ALMS){
		//Print some useful info to the console
		char consoleBuffer[1000] = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
		usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "FB: %ld\r\n", fb_pos);
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "FBT: %ld\r\n", fb_target);
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "LR: %ld\r\n", lr_pos);
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "LRT: %ld\r\n", lr_target);
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "UD: %ld\r\n", ud_pos);
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "S: %d\r\n", readEncoder(encoderSpindle));
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "FP: %d\r\n", readFrontPanel());
		//usart_write_line(CONF_UART, consoleBuffer);
		//sprintf(consoleBuffer, "P0: %d\r\n", readIO());
		//usart_write_line(CONF_UART, consoleBuffer);
		configure_rtt();
		}
	}
}

void updateEncoder(long int *pos, uint16_t encoderCounter){
	//Check for rollover
	if(abs((*pos & 0xFFFF) - encoderCounter) > 30000){
		//Check if roll down
		long int temp = *pos >> 16;
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

