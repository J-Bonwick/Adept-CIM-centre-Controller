#ifndef CORE_H_
#define CORE_H_

enum core_state {
  CORE_STATE_INIT,
  CORE_STATE_RUNNING,
  CORE_STATE_ESTOP,
  CORE_STATE_ERROR,
};

struct core {
  enum core_state state;
  struct {
    struct axis fb;
    struct axis lr;
    struct axis ud;
  } axis;
  struct {
    struct spindle lathe;
    struct spindle mill;
  } spindle;
  uint8_t lnm; /* Lathe Not Mill */
};

#endif // CORE_H_
