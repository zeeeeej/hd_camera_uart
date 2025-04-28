#include <stdio.h>
#include "hd_camera_uart_host.h"

int main() {
    printf("==========hd_camera_host==========\n");

    hd_camera_uart_host_init(PATH_HOST_DEFAULT);

    hd_camera_uart_host_deinit();
    return 0;
}
