#include "command_axis.h"

static int serial_command_axis_pos_get(struct acc *acc, int argc, char *argv[]);
static int serial_command_axis_pos_set(struct acc *acc, int argc, char *argv[]);
static int serial_command_axis_state(struct acc *acc, int argc, char *argv[]);
