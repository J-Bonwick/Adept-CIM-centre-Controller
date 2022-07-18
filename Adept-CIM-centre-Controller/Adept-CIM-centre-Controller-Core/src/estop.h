#ifndef ESTOP_H_
#define ESTOP_H_

/* Trigger estop incase of error
 * - error message can be NULL */
void estop_trigger(char *error);

#endif // ESTOP_H_
