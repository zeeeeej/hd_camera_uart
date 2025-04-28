#ifndef __HD_UART__
#define __HD_UART__

#include <stdint.h>

 int hd_uart_set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);

 int hd_uart_open(const char *com);

#endif // __HD_UART__