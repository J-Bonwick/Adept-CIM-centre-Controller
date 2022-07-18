/*
 * pid.h
 *
 * Created: 23/12/2020 12:07:35 PM
 *  Author: jbonwick
 */

#ifndef PID_H_
#define PID_H_

struct pid {
  float Kp;
  float Ki;
  float Kd;

  int limit_min;
  int limit_max;
  int limit_integral;

  float last_integral;
  int last_error;
};

// typedef struct pidController * pid_t;
void pid_configure(struct pid *pid, int *input, int *output, int *target,
                   float Kp, float Ki, float Kd, int minOutput, int maxOutput);
void pid_calculate(struct pid *pid);

#endif /* PID_H_ */
