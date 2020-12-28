/*
 * pid.h
 *
 * Created: 23/12/2020 12:07:35 PM
 *  Author: jbonwick
 */ 


#ifndef PID_H_
#define PID_H_

struct pid_t {
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

//typedef struct pidController * pid_t;
void pidConfigure(struct pid_t *pid, int *input, int *output, int *target, float Kp, float Ki, float Kd, int minOutput, int maxOutput);
void pidCalculate(struct pid_t *pid);

#endif /* PID_H_ */