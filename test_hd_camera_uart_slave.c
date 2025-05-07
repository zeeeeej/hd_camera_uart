#include <stdio.h>
#include "hd_camera_uart_slave.h"

// gcc -o hd_camera_slave hd_camera_protocol.c hd_camera_protocol_cmd.c hd_camera_uart_slave.c hd_uart.c test_hd_camera_uart_slave.c -lpthread
int main() {
    printf("==========hd_camera_slave==========\n");

    hd_camera_uart_slave_init("/dev/ttymxc5");

    hd_camera_uart_slave_deinit();
    return 0;
}
