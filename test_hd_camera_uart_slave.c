#include <stdio.h>
#include "hd_camera_uart_slave.h"

int main() {
    printf("==========hd_camera_slave==========\n");

    hd_camera_uart_slave_init("/dev/ttymxc5");

    hd_camera_uart_slave_deinit();
    return 0;
}
