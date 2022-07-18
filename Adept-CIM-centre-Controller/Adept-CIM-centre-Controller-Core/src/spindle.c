#include "spindle.h"
#include "cim_centre_io.h"
#include "utils.h"

struct spindle {
  int flags;
  struct {
    uint16_t input;
    uint16_t output;
  } rpm;
  uint16_t output;
  dac_t dac_addr;
  encoder_t encoder_addr;
};

void spindle_init(struct spindle *spindle, dac_t dac_addr,
                  encoder_t encoder_addr) {
  memset(spindle, 0, sizeof(*spindle));
  spindle->dac_addr = dac_addr;
  spindle->encoder_addr = encoder_addr
}

void spindle_rpm_set(struct spindle *spindle, float rpm) {
  spindle->rpm.output = rpm;
}

float spindle_rpm_get(struct spindle *spindle) { return spindle->rpm.input; }

int spindle_sync(struct spindle *spindle) {
  if (0 == spindle->flags) {
    return 0;
  }
  /*TODO: Get current rpm, calc pid output, set output */
  /* For now estimate rpm though hard coded values */

  /* map input rpm of 0-1200rpm to output */
  spindle->output = range_map(0, 1200, 0, 2048, spindle->rpm.output);

  dac_set(spindle->dac_addr, spindle->output);

  return 0;
}
