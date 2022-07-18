#include "axis.h"
#include "cim_centre_io.h"
#include "pid.h"

struct axis {
  int state;
  struct {
    int target;
    int current;
  } position;
  struct {
    int min;
    int max;
  } limits;
  uint8_t output;
  uint8_t speed;
  struct pid pid;
  uint16_t dac_addr;
  uint16_t encoder_addr;
};

void axis_init(struct axis *axis, uint16_t dac_addr) {
  memset(axis, 0, sizeof(*axis));
  axis->state = STATE_UNKNOWN;
  /* TODO: Set current pos to encoder pos. Set current target to current pos */
}

void axis_update_from_encoder(struct axis *axis, uint16_t encoder_pos) {
  int temp;
  int pos;

  pos = axis->position.current;
  // Check for rollover
  if (abs((pos & 0xFFFF) - encoder_pos) > 30000) {
    // Check if roll down
    temp = pos >> 16;
    if (encoder_pos & 0x8000) {
      temp--;
    } else {
      temp++;
    }
    pos = temp << 16;
    pos |= encoder_pos;
  } else {
    pos &= 0xFFFF0000;
    pos |= encoder_pos;
  }
  axis->position.current = pos;
}

int axis_set_target(struct axis *axis, int new_target) {
  if (new_target < axis->limits.min) {
    axis->position.target = axis->limits.min;
  } else if (new_target > axis->limits.max) {
    axis->position.target = axis->limits.max;
  }
  return axis->position.target;
}

int axis_get_target(struct axis *axis) { return axis->position.target; }
int axis_get_pos(struct axis *axis) { return axis->position.current; }

int axis_update_output(struct axis *axis) {
  if (!(axis->state & STATE_ENABLED)) {
    return 0;
  }

  /* Calculate PID and update the output */
  pid_calculate(&axis->pid);

  set_dac(axis->dac_addr, axis->output);

  return 0;
}

void axis_home(struct axis *axis) {}

void axix_enable(struct axis *axis) { axis->state |= STATE_ENABLED; }

void axix_disable(struct axis *axis) { axis->state &= ^STATE_ENABLED; }

int axis_get_state(struct axis *axis) { return axis->state; }
