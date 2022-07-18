#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <asf.h>

#include "axis.h"
#include "cim_centre_io.h"
#include "console.h"
#include "main.h"
#include "pid.h"
#include "spindle.h"

#define FB_MAX 198000

void configure_rtt(void) {
  rtt_init(RTT, 30);
  rtt_write_alarm_time(RTT, 5);
}

void main_init(struct global *global) { /* Init axis */
  /* axis init */
  axix_init(&global->axis.fb);
  axis_sync(&global->axis.fb);

  axix_init(&global->axis.lr);
  axis_sync(&global->axis.lr);

  axix_init(&global->axis.ud);
  axis_sync(&global->axis.ud);

  /* Spindle init */
  spindle_init(&global->spindle.lathe);
  spindle_sync(&global->spindle.lathe);

  spindle_sync(&global->spindle.mill);
  spindle_sync(&global->spindle.mill);
}

int main(void) {
  sysclk_init();
  board_init();

  configure_console();

  // Initialize io pins on the bus
  io_init();

  main_init(struct global)

      uint16_t frontPanelStatus = readFrontPanel();
  uint16_t generalIO = readIO();

  configure_rtt();
  bool hadAlarm = false;

  while (1) {
    // Check for RTT alarm
    if (rtt_get_status(RTT) & RTT_SR_ALMS) {
      hadAlarm = true;
      // Reset alarm
      configure_rtt();
    }

    /* Time sensitive tasks: */
    if (had_alarm) {
      axis_update_output(&axis_fb);
      axis_update_output(&axis_lr);
      axis_update_output(&axis_ud);
    }
    set_dacs(axis_lr.output, axis_fb.output, axis_ud.output,
             axis_spindle.output);

    frontPanelStatus = readFrontPanel();
    generalIO = readIO();

    // Check for movement buttons every 5ms
    if (hadAlarm) {
      if (frontPanelStatus & FP_Z_PLUS) {
        lr_target += 200;
      }
      if (frontPanelStatus & FP_Z_MINUS) {
        lr_target -= 200;
      }
      if (frontPanelStatus & FP_X_PLUS) {
        fb_target -= 200;
        if (fb_target < 0) {
          fb_target = 0;
        }
      }
      if (frontPanelStatus & FP_X_MINUS) {
        fb_target += 200;
      }
    }

    if (hadAlarm) {
      // Print some useful info to the console
      char consoleBuffer[100];
      // usart_write_line(CONF_UART, consoleBuffer);
      sprintf(consoleBuffer, "FB: %d\r\n", fb_current);
      usart_write_line(CONF_UART, consoleBuffer);
      sprintf(consoleBuffer, "FBT: %d\r\n", fb_target);
      usart_write_line(CONF_UART, consoleBuffer);
      sprintf(consoleBuffer, "LR: %d\r\n", lr_current);
      usart_write_line(CONF_UART, consoleBuffer);
      sprintf(consoleBuffer, "LRT: %d\r\n", lr_target);
      usart_write_line(CONF_UART, consoleBuffer);
      // sprintf(consoleBuffer, "UD: %d\r\n", ud_curent);
      // usart_write_line(CONF_UART, consoleBuffer);
      // sprintf(consoleBuffer, "S: %d\r\n", readEncoder(encoderSpindle));
      // usart_write_line(CONF_UART, consoleBuffer);
      // sprintf(consoleBuffer, "FP: %d\r\n", readFrontPanel());
      // usart_write_line(CONF_UART, consoleBuffer);
      sprintf(consoleBuffer, "P0: %d\r\n", readIO());
      usart_write_line(CONF_UART, consoleBuffer);
    }
  }
}
