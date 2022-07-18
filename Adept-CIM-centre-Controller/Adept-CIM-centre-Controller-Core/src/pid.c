/*
 * pid.c
 *
 * Created: 23/12/2020 12:07:22 PM
 *  Author: jbonwick
 */

#include "pid.h"

void pid_configure(struct pid *pid, int *input, int *output, int *target,
                   float Kp, float Ki, float Kd, int minOutput, int maxOutput) {
  pid->input = input;
  pid->output = output;
  pid->target = target;
  pid->Kp = Kp;
  pid->Ki = Ki;
  pid->Kd = Kd;
  pid->minValue = minOutput;
  pid->maxValue = maxOutput;
  pid->iTerm = 0;
  pid->lastInput = 0;
}

void pid_calculate(struct pid *pid) {
  // int input = *pid->input;
  // int error = *pid->target - input;
  ////Compute the integral
  // pid->iTerm += pid->Ki * error;
  ////Clamp iterm
  // if(pid->iTerm > pid->maxValue){
  // pid->iTerm = pid->maxValue;
  //}
  // else if(pid->iTerm < pid->minValue){
  // pid->iTerm = pid->minValue;
  //}
  //
  ////Compute differential
  // float dInput = input - pid->lastInput;
  //
  ////Compute the pid output
  // float output = pid->Kp * error + error + pid->iTerm - pid->Kd * dInput;
  ////Clamp output
  // if(output > pid->maxValue){
  // output = pid->maxValue;
  //}
  // else if(output < pid->minValue){
  // output = pid->minValue;
  //}
  //*pid->output = output;
  // pid->lastInput = input;

  int error = *pid->target - *pid->input;
  int output = pid->Kp * error;
  if (output > pid->maxValue) {
    output = pid->maxValue;
  } else if (output < pid->minValue) {
    output = pid->minValue;
  }
  *pid->output = output;
}
