#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hd_camera_protocol.h"

static uint16_t crc16(const uint8_t *data, uint32_t length) {
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

uint8_t hd_camera_protocol_decode(
        const unsigned char *recv_data_in,
        uint32_t recv_data_size_in,
         uint8_t *  slave_addr_out,
         uint8_t *  cmd_out,
         uint32_t *  payload_data_size_out,
         unsigned char **  payload_data_out
) {
    if (DEBUG) {
        hd_camera_protocol_print_buffer(recv_data_in, recv_data_size_in, "[decode]");
    }

    // 最小帧长度检查：帧头(2) + 地址(1) + 命令(1) + 长度(4) + CRC(2) = 10
    if (recv_data_size_in < 10) {
        return -1; // 数据长度不足
    }

    // 检查帧头
    if (recv_data_in[0] != PROTOCOL_HEADER_1 || recv_data_in[1] != PROTOCOL_HEADER_0) {
        printf("[uart]recv_data[0][1]:%02x,%02x\n", recv_data_in[0], recv_data_in[1]);
        return -2; // 帧头错误
    }

    // 获取从机地址
    *slave_addr_out = (uint8_t)recv_data_in[2];

    // 获取命令
    *cmd_out = recv_data_in[3];

    // 获取数据长度（小端格式）
    uint32_t length = (recv_data_in[7] << 24) | (recv_data_in[6] << 16) |
                      (recv_data_in[5] << 8) | recv_data_in[4];

    // 检查数据长度是否合理
    if (length > (recv_data_size_in - 10)) {
        return -4; // 数据长度超过实际数据
    }

    *payload_data_size_out = length;
    if (length > 0) {
        *payload_data_out = (unsigned char *)&recv_data_in[8];
    } else {
        *payload_data_out = NULL;
    }

    uint16_t received_crc = (recv_data_in[8 + length]) | recv_data_in[9 + length] << 8;
    uint16_t calculated_crc = crc16(recv_data_in, 8 + length);

    if (received_crc != calculated_crc) {
        printf("%02x vs %02x \n", received_crc, calculated_crc);
        return -5; // CRC校验失败
    }
    if (DEBUG){
        printf("->cmd = %u \n",*cmd_out);
        printf("->slave_addr = %u \n",*slave_addr_out);
    }
    return 0; // 解析成功
}

uint8_t hd_camera_protocol_encode(
        unsigned char **dest_data_output,
        uint32_t *dest_data_size_output,
        uint8_t slave_addr_in,
        uint8_t cmd_in,
        uint32_t payload_data_size_in,
        const unsigned char *payload_data_in
) {
    // 计算总数据长度: 帧头(2) + 地址(1) + 命令(1) + 长度(4) + payload(n) + CRC(2)
    const uint32_t fixed_header_size = 8; // 帧头+地址+命令+长度
    const uint32_t crc_size = 2;
    const uint32_t total_size = fixed_header_size + payload_data_size_in + crc_size;

    // 分配内存
    uint8_t *buffer = (unsigned char *) malloc(total_size);
    if (buffer == NULL) {
        return -1; // 内存分配失败
    }

    // 填充帧头 (0x5AA5)
    buffer[0] = PROTOCOL_HEADER_1;
    buffer[1] = PROTOCOL_HEADER_0;

    // 填充从机地址 (由参数指定)
    buffer[2] = slave_addr_in;

    // 填充命令
    buffer[3] = cmd_in;

    // 填充数据长度 (小端模式)
    buffer[4] = (payload_data_size_in >> 0) & 0xFF;  // LSB
    buffer[5] = (payload_data_size_in >> 8) & 0xFF;
    buffer[6] = (payload_data_size_in >> 16) & 0xFF;
    buffer[7] = (payload_data_size_in >> 24) & 0xFF; // MSB

    // 填充payload数据
    if (payload_data_size_in > 0 && payload_data_in != NULL) {
        memcpy(&buffer[8], payload_data_in, payload_data_size_in);
    }

    // 计算CRC (覆盖: 帧头到payload)
    uint16_t crc = crc16(buffer, fixed_header_size + payload_data_size_in);
    printf("crc = %02x \n", crc);

    // 填充CRC (小端模式)
    buffer[fixed_header_size + payload_data_size_in] = crc & 0xFF;      // LSB
    buffer[fixed_header_size + payload_data_size_in + 1] = (crc >> 8) & 0xFF; // MSB

    // 返回生成的数据
    *dest_data_output = buffer;
    *dest_data_size_output = total_size;
    if (DEBUG) {
        hd_camera_protocol_print_buffer(buffer, total_size, "[encode]");
    }
    return 0; // 成功

}


/**
 * 打印结果
 * [uart]<tag>12 34 56 78
 */
void hd_camera_protocol_print_buffer(const unsigned char *buf, uint32_t len, const char *tag) {
    // 最大支持打印256字节数据（每个字节3字符："XX "或"XX"）
    char output[256 * 3 + 20]; // 缓冲区留有余量
    char *pos = output;

    // 组装前缀
    pos += sprintf(pos, "[uart]<%s>", tag);

    // 组装十六进制数据
    for (uint32_t i = 0; i < len; i++) {
        pos += sprintf(pos, "%02X", buf[i]);
        if (i < len - 1) {
            pos += sprintf(pos, " ");
        }
    }

    // 一次性输出
    printf("%s\n", output);
}
//
//uint8_t hd_camera_protocol_cmd_heartbeat_req_encode(
//        unsigned char **protocol_data_out,
//        uint32_t *protocol_data_size_out,
//        uint8_t slave_addr_in,
//        uint8_t ack_number_in
//) {
//    uint8_t payload[] = {ack_number_in};
//    uint8_t ret = hd_camera_protocol_encode(
//            protocol_data_out,
//            protocol_data_size_out,
//            slave_addr_in,
//            CMD_HEARTBEAT,
//            sizeof(payload),
//            payload
//    );
//    return ret;
//}
//
//uint8_t hd_camera_protocol_cmd_heartbeat_resp_encode(
//        unsigned char **protocol_data_out,
//        uint32_t *protocol_data_size_out,
//        uint8_t slave_addr_in,
//        uint8_t ack_number_in
//) {
//    uint8_t payload[] = {ack_number_in};
//    uint8_t ret = hd_camera_protocol_encode(
//            protocol_data_out,
//            protocol_data_size_out,
//            slave_addr_in,
//            CMD_HEARTBEAT,
//            sizeof(payload),
//            payload
//    );
//    return ret;
//}
//
//uint8_t hd_camera_protocol_cmd_heartbeat_req_decode(
//        const unsigned char  *        protocol_data_in,
//        uint32_t                protocol_data_size_in,
//        uint8_t*                slave_addr_out,
//        uint8_t*                ack_number_out
//){
//    uint8_t cmd;
//    unsigned char *payload_data;
//    uint32_t payload_data_size;
//    uint8_t ret = hd_camera_protocol_decode(protocol_data_in, protocol_data_size_in, slave_addr_out, &cmd, &payload_data_size,
//                                            &payload_data);
//
//    if (DEBUG){
//        //printf("ret = %u payload_data_size=%u\n",ret,payload_data_size);
//        hd_camera_protocol_print_buffer(payload_data,payload_data_size,"hd_camera_protocol_cmd_heartbeat_req_decode::result");
//    }
//
//    if (ret != 0) return -1;
//    if (payload_data_size == 1) {
//        *ack_number_out = payload_data[0];
//        return 0;
//    } else {
//        return -1;
//    }
//}
//
//uint8_t hd_camera_protocol_cmd_heartbeat_resp_decode(
//        const unsigned char *protocol_data_in,
//        uint32_t protocol_data_size_in,
//        uint8_t *slave_addr_out,
//        uint8_t *ack_number_out
//) {
//    uint8_t cmd;
//    unsigned char *payload_data;
//    uint32_t payload_data_size;
//    uint8_t ret = hd_camera_protocol_decode(protocol_data_in, protocol_data_size_in, slave_addr_out, &cmd, &payload_data_size,
//                                            &payload_data);
//
//    if (DEBUG){
//        //printf("ret = %u payload_data_size=%u\n",ret,payload_data_size);
//        hd_camera_protocol_print_buffer(payload_data,payload_data_size,"hd_camera_protocol_cmd_heartbeat_resp_decode::result");
//    }
//
//    if (ret != 0) return -1;
//    if (payload_data_size == 1) {
//        *ack_number_out = payload_data[0];
//        return 0;
//    } else {
//        return -1;
//    }
//}
//
//
//uint8_t hd_camera_protocol_cmd_property_get_req(uint8_t slave_addr, uint8_t property_id) {
//    return 0;
//}
//
//uint8_t
//hd_camera_protocol_cmd_property_get_resp(uint8_t *slave_addr, uint32_t *len, uint8_t *property_id, uint8_t *result,
//                                         unsigned char **property_value) {
//    return 0;
//}
//
//uint8_t
//hd_camera_protocol_cmd_property_set_req(uint8_t slave_addr, uint8_t property_id, const unsigned char *property_value,
//                                        uint32_t property_value_size) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_set_resp(uint8_t *slave_addr, uint8_t *property_id, uint8_t *result) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_recovery_req(uint8_t slave_addr) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_recovery_resp(uint8_t *slave_addr, uint8_t *result) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_reboot_req(uint8_t slave_addr) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_reboot_resp(uint8_t *slave_addr, uint8_t *result) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_snapshot_req(uint8_t slave_addr) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_snapshot_resp(uint8_t *slave_addr, uint8_t *result, uint8_t *pic_id) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_info_req(uint8_t slave_addr) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_info_resp(uint8_t *slave_addr, uint32_t *len, uint8_t *pic_num,
//                                                         hd_camera_protocol_pic_info **pic_infos) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_delete_req(uint8_t slave_addr, uint8_t pic_id) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_delete_resp(uint8_t *slave_addr, uint8_t *result) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_pull_req(uint8_t slave_addr, uint8_t pic_id, uint32_t offset,
//                                                        uint32_t read_len) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_pull_resp(uint8_t *slave_addr, uint32_t *len, uint8_t *result,
//                                                         unsigned char **pic_data) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_pull_completed_req(uint8_t slave_addr, uint8_t pic_id) {
//    return 0;
//}
//
//uint8_t
//hd_camera_protocol_cmd_property_camera_pull_completed_resp(uint8_t *slave_addr, uint8_t pic_id, uint8_t *result) {
//    return 0;
//}
//
//uint8_t
//hd_camera_protocol_cmd_property_camera_ota_req(uint8_t slave_addr, uint32_t file_size, const unsigned char *file_md5) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_ota_resp(uint8_t *slave_addr, uint8_t *result, uint32_t *offset) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_ota_push_req(uint8_t slave_addr, uint32_t offset, uint32_t data_len,
//                                                            const unsigned char *data) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_ota_push_resp(uint8_t *slave_addr, uint8_t *result) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_door_signal_req(uint8_t slave_addr) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_door_signal_resp(uint8_t *slave_addr, uint8_t *result) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_req(uint8_t slave_addr, uint32_t file_size,
//                                                               unsigned char *file_md5) {
//    return 0;
//}
//
//uint8_t
//hd_camera_protocol_cmd_property_camera_app_upgrade_resp(uint8_t *slave_addr, uint8_t *result, uint32_t *offset) {
//    return 0;
//}
//
//uint8_t
//hd_camera_protocol_cmd_property_camera_app_upgrade_push_req(uint8_t slave_addr, uint32_t offset, uint32_t data_len,
//                                                            unsigned char *data) {
//    return 0;
//}
//
//uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_push_resp(uint8_t *slave_addr, uint8_t *result) {
//    return 0;
//}
//
//
//typedef struct {
//
//    uint8_t id;
//    /**
//     * 数据类型：
//     * 0:uint8_t
//     * 1:string
//     * 2:uint32_t
//     * 3:struct
//     */
//    uint8_t type;
//    /**
//     * 数据长度
//     * -1 : n
//     */
//    u_int8_t len;
//    /**
//     * 读写
//     * 0b11
//     * bit0:读
//     * bit1:写
//     */
//    u_int8_t rw;
//} HDCameraProperty;
