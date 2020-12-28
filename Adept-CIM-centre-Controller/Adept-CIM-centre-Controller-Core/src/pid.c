/*
 * pid.c
 *
 * Created: 23/12/2020 12:07:22 PM
 *  Author: jbonwick
 */ 

#include "pid.h"

void pidCalculate(pid_t pid){
	int input = *pid->input;
	int error = *pid->target - input;
	//Compute the integral
	pid->iTerm += pid->Ki * error;
	//Clamp iterm
	if(pid->iTerm > pid->maxValue){
		pid->iTerm = pid->maxValue;
	}
	else if(pid->iTerm < pid->minValue){
		pid->iTerm = pid->minValue;
	}
	
	//Compute differential
	float dInput = input - pid->lastInput;
	
	//Compute the pid output
	float output = pid->Kp * error + error + pid->iTerm - pid->Kd * dInput;
	//Clamp output
	if(output > pid->maxValue){
		output = pid->maxValue;
	}
	else if(output < pid->minValue){
		output = pid->minValue;
	}
	*pid->output = output;
	pid->lastInput = input;
}