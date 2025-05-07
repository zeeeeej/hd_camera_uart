
#ifndef __HD_CAMERA_PROTOCOL_CMD__
#define __HD_CAMERA_PROTOCOL_CMD__

#include <stdint.h>

#define HEARTBEAT_INTERVAL              5 // 单位:秒

// cmds
#define CMD_HEARTBEAT                   0x01
#define CMD_PROPERTY_GET                0x02
#define CMD_RECOVERY_SET                0x03
#define CMD_RECOVERY                    0x04
#define CMD_CAMERA_REBOOT               0x05
#define CMD_CAMERA_SNAPSHOT             0x06
#define CMD_CAMEAR_INFO                 0x07
#define CMD_CAMEAR_DELETE               0x08
#define CMD_CAMEAR_PULL                 0x09
#define CMD_CAMEAR_PULL_COMPLETED       0x0A
#define CMD_OTA                         0x0B
#define CMD_OTA_PUSH                    0x0C
#define CMD_DOOR_SIGNAL                 0x0D
#define CMD_APP_UPGRADE                 0x1B
#define CMD_APP_UPGRADE_PUSH            0x1C

/**
 * 心跳 请求
 *
 * ｜ 0x01 | 0x01 | N |
 *
 * @param slave_addr_in        从机地址
 * @param ack_number_in        请求序号
 */
uint8_t hd_camera_protocol_cmd_heartbeat_req_encode(
        unsigned char **    protocol_data_out,
        uint32_t *          protocol_data_size_out,
        uint8_t             slave_addr_in,
        uint8_t             ack_number_in
);

/**
 * 心跳 应答
 *
 * ｜ 0x01 | 0x01 | N+1 |
 *
 * @param slave_addr_in        从机地址
 * @param ack_number_in        返回序号 = 请求序号+1
 */
uint8_t hd_camera_protocol_cmd_heartbeat_resp_encode(
        unsigned char **    protocol_data_out,
        uint32_t *          protocol_data_size_out,
        uint8_t             slave_addr_in,
        uint8_t             ack_number_in
);

uint8_t hd_camera_protocol_cmd_heartbeat_req_decode_with_data(
        const unsigned char  *        protocol_data_in,
        uint32_t                protocol_data_size_in,
        uint8_t*                slave_addr_out,
        uint8_t*                ack_number_out
);

uint8_t hd_camera_protocol_cmd_heartbeat_req_decode(
        const unsigned char  *        payload_data_in,
        uint32_t                payload_data_size_in,
        uint8_t*                ack_number_out
);



/**
 * 解析心跳 应答
 * @param protocol_data_in          整条协议数据
 * @param protocol_data_size_in     整条协议数据大小
 * @param slave_addr_out            地址
 * @param ack_number_out            序号
 * @return
 */
uint8_t hd_camera_protocol_cmd_heartbeat_resp_decode_with_data(
        const unsigned char  *        protocol_data_in,
        uint32_t                protocol_data_size_in,
        uint8_t*                slave_addr_out,
        uint8_t*                ack_number_out
);

/**
 * 解析心跳 应答
 * @param payload_data_in            payload
 * @param payload_data_size_in       payload_size
 * @param ack_number_out        序号
 */
uint8_t hd_camera_protocol_cmd_heartbeat_resp_decode(
        const unsigned char  *        payload_data_in,
        uint32_t                payload_data_size_in,
        uint8_t*                ack_number_out
);

/**
 * 查询属性 请求
 *
 * | 0x02 | 0x01 | ID |
 *
 * @param slave_addr        从机地址
 * @param property_id       属性ID
 */
uint8_t hd_camera_protocol_cmd_property_get_req_encode(uint8_t slave_addr, uint8_t property_id);

/**
 * 查询属性 应答
 *
 * | 0x02 | len | ID | result | value |
 *
 * @param slave_addr        从机地址
 * @param len               数据长度 = property_id + result + property_value
 * @param property_id       属性ID
 * @param result            返回值。0x00：成功，其他：失败
 * @param property_value    属性值
 */
uint8_t hd_camera_protocol_cmd_property_get_resp_encode(
    uint8_t *slave_addr,
    uint32_t *len,
    uint8_t *property_id,
    uint8_t *result,
    unsigned char ** property_value
);

//uint8_t hd_camera_protocol_cmd_property_get_resp_decode(
//        uint8_t *slave_addr,
//        uint32_t *len,
//        uint8_t *property_id,
//        uint8_t *result,
//        unsigned char ** property_value
//);

/**
 * 设置属性 请求
 *
 * | 0x03 | len | ID | value |
 *
 * @param slave_addr        从机地址
 * @param property_id       属性ID
 * @param property_value    属性值
 * @param property_value    属性值size
 */
uint8_t hd_camera_protocol_cmd_property_set_req(
        uint8_t slave_addr,
        uint8_t property_id,
        const unsigned char * property_value,
        uint32_t property_value_size
);

/**
 * 设置属性 应答
 *
 * | 0x03 | 0x02 | ID | result |
 *
 * @param slave_addr        从机地址
 * @param property_id       属性ID
 * @param result            返回值。0x00：成功，其他：失败
 */
uint8_t hd_camera_protocol_cmd_property_set_resp(
    uint8_t *slave_addr,
    uint8_t *property_id,
    uint8_t *result
);

/**
 * 恢复出厂 请求
 *
 * | 0x04 | 0x00 |
 *
 * @param slave_addr        从机地址
 */
uint8_t hd_camera_protocol_cmd_property_recovery_req(uint8_t slave_addr);

/**
 * 恢复出厂 应答
 *
 * | 0x04| 0x01 | result |
 *
 * @param slave_addr        从机地址
 * @param result            返回值。0x00：成功，其他：失败
 */
uint8_t hd_camera_protocol_cmd_property_recovery_resp(
    uint8_t *slave_addr,
    uint8_t *result
);

/**
 * 重启摄像头 请求
 *
 * | 0x05 | 0x00 |
 *
 * @param slave_addr        从机地址
 */
uint8_t hd_camera_protocol_cmd_property_camera_reboot_req(uint8_t slave_addr);

/**
 * 重启摄像头 应答
 *
 * | 0x05 | 0x00 |
 *
 * @param slave_addr        从机地址
 * @param result            返回值。0x00：成功，其他：失败
 */

uint8_t hd_camera_protocol_cmd_property_camera_reboot_resp(
    uint8_t *slave_addr,
    uint8_t *result
);

/**
 * 主动抓图 请求
 *
 * | 0x06 | 0x00 |
 *
 * @param slave_addr        从机地址
 */
uint8_t hd_camera_protocol_cmd_property_camera_snapshot_req(uint8_t slave_addr);
/**
 * 主动抓图 应答
 *
 * | 0x06 | 0x02 | result | pic_id |
 *
 * @param result            返回值。0x00：成功，其他：失败
 * @param pic_id            图片id，取图片用
 */
uint8_t hd_camera_protocol_cmd_property_camera_snapshot_resp(
    uint8_t *slave_addr,
    uint8_t *result,
    uint8_t *pic_id
);

typedef struct {
    /** pic_id 1BYTE */
    uint8_t id;
    /** 触发方式 1BYTE */
    uint8_t trigger_type;
    /** 触发角度 1BYTE */
    uint8_t trigger_angle;
    /** 抓取时间 4BYTE */
    uint32_t snapshot_timestamps;
    /** 图片大小 4BYTE */
    uint32_t size;
    /** MD5 16BYTE */
    unsigned char  md5[16];
} hd_camera_protocol_pic_info;

/**
 * 查询摄像头存贮的图片信息 请求
 *
 * | 0x07 | 0x00 |
 *
 * @param slave_addr        从机地址
 */
uint8_t hd_camera_protocol_cmd_property_camera_info_req(uint8_t slave_addr);

/**
 * 查询摄像头存贮的图片信息 应答
 *
 * | 0x07 | len | pic_num | pic_info0 ... pic_infoN |
 *
 * @param slave_addr        从机地址
 * @param len               1 + pic_num * 27
 * @param pic_num           图片数量
 * @param pic_infos         pic_info [ pic_id（1BYTE） ｜ 触发方式（1BYTE） ｜ 触发角度（1BYTE） ｜ 抓取时间（4BYTE） ｜ 图片大小（4BYTE） ｜ MD5（16BYTE） ]
 */
uint8_t hd_camera_protocol_cmd_property_camera_info_resp(
    uint8_t *slave_addr,
    uint32_t *len,
    uint8_t *pic_num,
    hd_camera_protocol_pic_info **pic_infos
);

/**
 * 删除图片 请求
 *
 * | 0x08 | 0x01 | pic_id |
 *
 * @param slave_addr        从机地址
 * @param pic_id            图片id
 */
uint8_t hd_camera_protocol_cmd_property_camera_delete_req(uint8_t slave_addr,uint8_t pic_id);

/**
 * 删除图片 请求
 *
 * | 0x08 | 0x01 | result |
 *
 * @param slave_addr        从机地址
 * @param result            返回值。0x00：成功，其他：失败
 */
uint8_t hd_camera_protocol_cmd_property_camera_delete_resp(
    uint8_t *slave_addr,
    uint8_t *result
);

/**
 * 获取图片 请求
 *
 * | 0x09 | 0x01 | pic_id | offset | read_len |
 *
 * @param slave_addr        从机地址
 * @param pic_id            图片id
 * @param offset            表示从图片数据的第offset字节开始读取
 * @param read_len          表示读取字节个数
 */
uint8_t hd_camera_protocol_cmd_property_camera_pull_req(uint8_t slave_addr,uint8_t pic_id,uint32_t offset,uint32_t read_len);

/**
 * 获取图片 应答
 *
 * | 0x09 | len | result | pic_data |
 *
 * @param slave_addr        从机地址
 * @param len               数据长度 = result + pic_data
 * @param result            返回值。0x00：成功，其他：失败
 * @param pic_data          读取的图片数据
 */
uint8_t hd_camera_protocol_cmd_property_camera_pull_resp(
    uint8_t *slave_addr,
    uint32_t *len,
    uint8_t *result,
    unsigned char **pic_data
);


/**
 * <*>获取图片完成 请求
 *
 * | 0x0A | 0x00 |
 *
 *  @param slave_addr        从机地址
 *  @param pic_id            图片id <*>
 */
uint8_t hd_camera_protocol_cmd_property_camera_pull_completed_req(uint8_t slave_addr,uint8_t pic_id);

/**
 * <*>获取图片完成 应答
 *
 * | 0x0A | 0x00 |
 *
 *  @param slave_addr        从机地址
 *  @param pic_id            图片id <*>
 *  @param result            返回值。0x00：成功，其他：失败
 */
uint8_t hd_camera_protocol_cmd_property_camera_pull_completed_resp(
    uint8_t *slave_addr,
    uint8_t pic_id,
    uint8_t *result
);

/**
 * 固件升级通知 请求
 *
 * | 0x0B | 0x14 | fileSize | fileMd5 |
 *
 *  @param slave_addr        从机地址
 *  @param file_size         固件大小。4字节
 *  @param file_md5          固件MD5。16字节
 */
uint8_t hd_camera_protocol_cmd_property_camera_ota_req(
        uint8_t slave_addr,
        uint32_t file_size,
        const unsigned char  file_md5[16]
);

/**
 * 固件升级通知 应答
 *
 * | 0x0B | 0x05 | result | offset |
 *
 * 相机收到通知后，首次传输固件返回offset=0。如果上次传输中断，MD5与上次一致，返回offset=已接收固件大小，进行断点续传。
 * 主机从第offset字节开始，分包发送固件。
 *
 *  @param slave_addr        从机地址
 *  @param result            返回值。0x00：可以固件升级，其他：升级未准备就绪
 *  @param offset            已接收固件大小（偏移量），范围0~fileSize。用于断点续传。
 */
uint8_t hd_camera_protocol_cmd_property_camera_ota_resp(
    uint8_t *slave_addr,
    uint8_t *result,
    uint32_t *offset
);

/**
 * 发送固件升级包 请求
 *
 * | 0x0C | len | offset | dataLen | data |
 *
 * @param slave_addr        从机地址
 * @param offset            固件数据包偏移量
 * @param data_len          0x000000000：发送完毕;0xFFFFFFFF:  终止OTA升级;其他：固件数据包长度。
 * @param data              固件数据包
 */
uint8_t hd_camera_protocol_cmd_property_camera_ota_push_req(
        uint8_t slave_addr,
        uint32_t offset,
        uint32_t data_len,
        const unsigned char * data
);

/**
 * 发送固件升级包 应答
 *
 * | 0x0C | 0x01 | result |
 *
 * @param slave_addr        从机地址
 * @param result            0x00：完整固件接收完毕。0x01：发送下一包数据；0x02：重发；其他：OTA失败，终止升级
 */
uint8_t hd_camera_protocol_cmd_property_camera_ota_push_resp(
    uint8_t *slave_addr,
    uint8_t *result
);

/**
 * 门开关信号 请求
 *
 * | 0x0D | 0x00 |
 *
 * @param slave_addr        从机地址
 */
uint8_t hd_camera_protocol_cmd_property_camera_door_signal_req(uint8_t slave_addr);

/**
 * 门开关信号 应答
 *
 * | 0x0D| 0x01 | result |
 *
 * @param slave_addr        从机地址
 * @param result            返回值。0x00：门关状态，0x01：门开状态
 */
uint8_t hd_camera_protocol_cmd_property_camera_door_signal_resp(
    uint8_t *slave_addr,
    uint8_t *result
);

/**
 * app升级通知 请求
 *
 * | 0x1B | 0x14 | fileSize | fileMd5 |
 *
 *  @param slave_addr        从机地址
 *  @param file_size         app大小。4字节
 *  @param file_md5          appMD5。16字节
 */
uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_req(
        uint8_t slave_addr,
        uint32_t file_size,
        unsigned char  file_md5[16]
);

/**
 * app升级通知 应答
 *
 * | 0x1B | 0x05 | result | offset |
 *
 * 相机收到通知后，首次传输app返回offset=0。如果上次传输中断，MD5与上次一致，返回offset=已接收app大小，进行断点续传。
 * 主机从第offset字节开始，分包发送app。
 *
 *  @param slave_addr        从机地址
 *  @param result            返回值。0x00：可以app升级，其他：升级未准备就绪
 *  @param offset            已接收app大小（偏移量），范围0~fileSize。用于断点续传。
 */
uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_resp(
    uint8_t *slave_addr,
    uint8_t *result,
    uint32_t *offset
);

/**
 * 发送app升级包 请求
 *
 * | 0x1C | len | offset | dataLen | data |
 *
 * @param slave_addr        从机地址
 * @param offset            app数据包偏移量
 * @param data_len          0x000000000：发送完毕;0xFFFFFFFF:  终止OTA升级;其他：app数据包长度。
 * @param data              app数据包
 */
uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_push_req(
        uint8_t slave_addr,
        uint32_t offset,
        uint32_t data_len,
        unsigned char * data
);

/**
 * 发送app升级包 应答
 *
 * | 0x1C | 0x01 | result |
 *
 * APP升级通知（0x1B）收到应答后，不再先发送请求，而是直接给摄像头发送升级包原始数据，摄像头接收完原始数据后仍需应答告知升级结果.
 *
 * @param slave_addr        从机地址
 * @param result            0x00：完整app接收完毕。0x01：发送下一包数据；0x02：重发；其他：OTA失败，终止升级
 */
uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_push_resp(
    uint8_t *slave_addr,
    uint8_t *result
);

#endif // __HD_CAMERA_PROTOCOL_CMD__
