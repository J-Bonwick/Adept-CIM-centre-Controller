#ifndef MAIN_H_
#define MAIN_H_

#include "core.h"

void core_sync(struct core *core) {
  /* Sync io */

  /* Sync front panel*/

  /* Sync motors */
  axis_sync(&core->axis.fb);
  axis_sync(&core->axis.lr);
  axis_sync(&core->axis.ud);
  spindle_sync(&core->spindle.mill);
  spindle_sync(&core->spindle.lathe);
}

void core_init(struct core *core) {
  memset(core, 0, sizeof(*core));
  core->state = CORE_STATE_INIT;

  axix_init(&core->axis.fb);
  axis_sync(&core->axis.fb);

  axix_init(&core->axis.lr);
  axis_sync(&core->axis.lr);

  axix_init(&core->axis.ud);
  axis_sync(&core->axis.ud);

  /* Spindle init */
  spindle_init(&core->spindle.lathe, DAC_SPINDLE, ENCODER_SPINDLE_LATHE);
  spindle_sync(&core->spindle.lathe);

  spindle_sync(&core->spindle.mill, DAC_SPINDLE, ENCODER_SPINDLE_MILL);
  spindle_sync(&core->spindle.mill);
}

#endif // MAIN_H_
