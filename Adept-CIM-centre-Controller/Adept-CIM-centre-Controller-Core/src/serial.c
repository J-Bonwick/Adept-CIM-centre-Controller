#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "serial.h"
#include "spindle.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define COMMAND_DELIM ' '

#define COMMAND_RESULT_OK (0)
#define COMMAND_RESULT_EARGS (1)
#define COMMAND_RESULT_ERANGE (2)

/* Spindle */
#define SPINDLE_LATHE_NAME "L"
#define SPINDLE_MILL_NAME "M"

struct serial_command {
  char *command;
  int argc; /* Number of expected arguments. Set to -1 to indicate a variable
               amount */
  char *(*fn)(struct acc *acc, int argc, char *argv[]);
};

int serial_command_print_integer(struct acc *acc, int value) {
  /* TODO async write support? */
}

static int serial_command_help(char *command, int argc, char *argv[]) {
  /* TODO: Return help */
  return 0;
}
static int serial_command_verion(char *command, int argc, char *argv[]) {
  /* TODO: Return version */
  return 0;
}

static int serial_command_axis_pos_set(struct acc *acc, int argc,
                                       char *argv[]) {

  return 0;
}

static int serial_command_axis_pos_get(struct acc *acc, int argc,
                                       char *argv[]) {
  return 0;
}

/* static int serial_command_axis_state(struct acc *acc, int argc, char *argv[])
 * { */
/*   return 0; */
/* } */

static struct spindle *get_spindle_from_str(struct acc *acc, char *spindle) {
  if (0 == strcmp(spindle, SPINDLE_LATHE_NAME)) {
    return &acc->spindle.lathe;
  } else if (0 == strcmp(spindle, SPINDLE_MILL_NAME)) {
    return &acc->spindle.spindle;
  } else {
    return NULL;
  }
}

static int serial_command_spindle_speed_set(struct acc *acc, int argc,
                                            char *argv[]) {
  struct spindle *spindle;
  int speed;

  spindle = get_spindle_from_str(acc, argv[0]);
  if (NULL == spindle) {
    return COMMAND_RESULT_EARGS;
  }

  errno = 0;
  speed = strtol(argv[1], NULL, 10);
  if (errno != 0) {
    return COMMAND_RESULT_EARGS;
  }

  if (-1 == spindle_speed_set(spindle, speed)) {
    return COMMAND_RESULT_ERANGE;
  }
  return 0;
}

static int serial_command_spindle_speed_get(struct acc *acc, int argc,
                                            char *argv[]) {
  int speed;
  struct spindle *spindle;

  spindle = get_spindle_from_str(acc, argv[0]);
  if (NULL == spindle) {
    return -1;
  }

  speed = spindle_speed_get_current(spindle);
  serial_command_print_integer(acc, speed);

  return 0;
}

static int serial_command_spindle_pos_get(struct acc *acc, int argc,
                                          char *argv[]) {
  return 0;
}

static const struct serial_command commands[] = {
    /* General */
    {.command = "H", .argc = 0, .fn = serial_command_help},
    {.command = "I", .argc = 0, .fn = serial_command_verion},
    /* Axis */
    {.command = "APS", .argc = 2, .fn = serial_command_axis_pos_set},
    {.command = "APG", .argc = 1, .fn = serial_command_axis_pos_get},
    /* {.command = "AS", .argc = 1, .fn = serial_command_axis_state}, */
    /* Spindle */
    {.command = "SSS", .argc = 1, .fn = serial_command_spindle_speed_set},
    {.command = "SSG", .argc = 0, .fn = serial_command_spindle_speed_get},
    {.command = "SPG", .argc = 0, .fn = serial_command_spindle_pos_get},
    /* {.command = "SS", .argc = 0, .fn = serial_command_spindle_state}, */
};

int serial_process_line(char *line) {
  line = strtok(line, COMMAND_DELIM);
  for (int i = 0; i < ARRAY_LENGTH(commands); i++) {
    if (strcmp(line, &commands[i])) {
      if (commands[i].argc != argc) {
        return COMMAND_RESULT_EARGS;
      }
      return commands[i].fn(acc, argc, argv);
    }
  }
}
