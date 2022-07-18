#ifndef SERIAL_H_
#define SERIAL_H_

#define LOG_EMERG 0
#define LOG_WARNING 4
#define LOG_INFO 6
#define LOG_DEBUG 7

/* Logs a message to the serial port
 * Supported levels: */
void serial_log(int level, char *msg);

#endif // SERIAL_H_
