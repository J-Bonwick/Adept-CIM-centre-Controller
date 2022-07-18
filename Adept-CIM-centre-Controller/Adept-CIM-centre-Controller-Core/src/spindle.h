#ifndef SPINDLE_H_
#define SPINDLE_H_

#define SPINDLE_STATE_DISABLED (0)
#define SPINDLE_STATE_ENABLED bit(0)

struct spindle;

/* TODO How to specify pid params per spindle? */
/* Initialise spindle object to disabled */
void spindle_init(struct spindle *spindle, dac_t dac_addr,
                  encoder_t encoder_addr);

/* Set spindle speed.*/
void spindle_rpm_set(struct spindle *spindle, float rpm);

/* Returns the current speed */
float spindle_rpm_get(struct spindle *spindle);

/* Call this function every xms */
int spindle_sync(struct spindle *spindle);

#endif // SPINDLE_H_
