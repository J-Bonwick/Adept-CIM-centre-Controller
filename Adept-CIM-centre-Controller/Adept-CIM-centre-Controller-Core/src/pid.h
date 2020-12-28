/*
 * pid.h
 *
 * Created: 23/12/2020 12:07:35 PM
 *  Author: jbonwick
 */ 


#ifndef PID_H_
#define PID_H_

struct pidController {
	int *input;
	int *output;
	int *target;
	
	float Kp;
	float Ki;
	float Kd;
	
	int minValue;
	int maxValue;
	
	float iTerm;
	int lastInput;
};

typedef struct pidController *pid_t;

void pidCalculate(pid_t pid);

#endif /* PID_H_ */