
#ifndef __HD_CAMERA_PROTOCOL__
#define __HD_CAMERA_PROTOCOL__

#include <stdint.h>

#define DEBUG         1

#define PROTOCOL_HEADER_0               0x5A        // 协议头标识0
#define PROTOCOL_HEADER_1               0xAA        // 协议头标识1
#define PROTOCOL_SLAVE_ADDR_DEFAULT     0x01        // 默认从机地址
#define PROTOCOL_RATE_DEFAULT           9600      // 串口波特率 // delete

static uint16_t crc16(const uint8_t *data, uint32_t length);

uint8_t hd_camera_protocol_decode(
    const unsigned char*    recv_data_in,
    uint32_t                recv_data_size_in,
     uint8_t*               slave_addr_out,
     uint8_t *               cmd_out,
     uint32_t *             payload_data_size_out,
     unsigned char**        payload_data_out
);


uint8_t hd_camera_protocol_encode(
    unsigned char**     dest_data_output,
    uint32_t*           dest_data_size_output,
    uint8_t             slave_addr_in,
    uint8_t             cmd_in,
    uint32_t            payload_data_size_in,
    const  unsigned char*    payload_data_in
);


void hd_camera_protocol_print_buffer(const unsigned char *buf, uint32_t len,const char * tag);

#endif // __HD_CAMERA_PROTOCOL__
