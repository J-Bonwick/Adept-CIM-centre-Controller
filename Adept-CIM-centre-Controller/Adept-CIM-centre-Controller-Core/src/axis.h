#ifndef AXIS_H_
#define AXIS_H_

#include "pid.h"

/* Homing logic follows the squence:
   - Drive axis until axis end_stop is triggered.
   - Drve axis back until index is triggered.
   - Reset relative position to 0.
 */
#define STATE_UNKNOWN (1U << 0)
#define STATE_HOMED (1U << 1)
#define STATE_HOMING_END (1U << 2)
#define STATE_HOMING_INDEX (1U << 3)
#define STATE_ERROR (1U << 4)
#define STATE_ENABLED (1U << 5)

struct axis;

void axis_init(struct axis *axis);
void axis_sync(struct axis *axis);

/* Updates the internal position from a raw encoder value.
 *
 * Note: Things will go horribly wrong if encoder value rolls over before this
 * function is called */
void axis_update_from_encoder(struct axis *axis, uint16_t encoder_pos);

/* Calcualtes new PID output.
 * This should be called on a timer at a regular interval */
int axis_update_output(struct axis *axis);

void axis_home(struct axis *axis);

/* - Sets a new target position
 * - If the new target position is outside of the min/max then it will be set
 * the to min/max.
 *
 * - Returns the new target position. */
int axis_set_target(struct axis *axis, int new_target);
int axis_get_target(struct axis *axis);
int axis_get_pos(struct axis *axis);

/*
  - Set all axis inputs and oututs to 0
  - Set default state to UNKNOWN
  - Sets up PID.
*/

void axix_enable(struct axis *axis);
void axix_disable(struct axis *axis);

int axis_get_state(struct axis *axis);

#endif
