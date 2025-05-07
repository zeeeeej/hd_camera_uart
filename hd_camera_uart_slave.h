#ifndef __HD_CAMERA_UART_SLAVE__
#define __HD_CAMERA_UART_SLAVE__

#include <stdint.h>

/**
 * 初始化摄像头SLAVE串口通信
 * @param path 串口设备路径(如"/dev/ttyUSB0")
 * @return 成功返回0，失败返回错误码(负值)
 */
int hd_camera_uart_slave_init(const char *path);

/**
 * 反初始化摄像头SLAVE串口通信
 * @return 成功返回0，失败返回错误码
 */
void hd_camera_uart_slave_deinit(void);

#endif // __HD_CAMERA_UART_SLAVE__