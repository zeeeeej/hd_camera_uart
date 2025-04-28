#ifndef __HD_CAMERA_UART_HOST__
#define __HD_CAMERA_UART_HOST__

#include <stdint.h>

#define PATH_HOST_DEFAULT  "/dev/tty.usbserial-2140"

/**
 * 初始化摄像头HOST串口通信
 * @param path 串口设备路径(如"/dev/ttyUSB0")
 * @return 成功返回0，失败返回错误码(负值)
 */
int hd_camera_uart_host_init(const char *path);

/**
 * 反初始化摄像头HOST串口通信
 * @return 成功返回0，失败返回错误码
 */
void hd_camera_uart_host_deinit(void);

#endif // __HD_CAMERA_UART_HOST__