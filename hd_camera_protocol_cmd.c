#include <stdint.h>
#include "hd_camera_protocol_cmd.h"
#include "hd_camera_protocol.h"

uint8_t hd_camera_protocol_cmd_heartbeat_req_encode(
        unsigned char **protocol_data_out,
        uint32_t *protocol_data_size_out,
        uint8_t slave_addr_in,
        uint8_t ack_number_in
) {
    uint8_t payload[] = {ack_number_in};
    uint8_t ret = hd_camera_protocol_encode(
            protocol_data_out,
            protocol_data_size_out,
            slave_addr_in,
            CMD_HEARTBEAT,
            sizeof(payload),
            payload
    );
    return ret;
}

uint8_t hd_camera_protocol_cmd_heartbeat_resp_encode(
        unsigned char **protocol_data_out,
        uint32_t *protocol_data_size_out,
        uint8_t slave_addr_in,
        uint8_t ack_number_in
) {
    uint8_t payload[] = {ack_number_in};
    uint8_t ret = hd_camera_protocol_encode(
            protocol_data_out,
            protocol_data_size_out,
            slave_addr_in,
            CMD_HEARTBEAT,
            sizeof(payload),
            payload
    );
    return ret;
}

uint8_t hd_camera_protocol_cmd_heartbeat_req_decode(
        const unsigned char  *        protocol_data_in,
        uint32_t                protocol_data_size_in,
        uint8_t*                slave_addr_out,
        uint8_t*                ack_number_out
){
    uint8_t cmd;
    unsigned char *payload_data;
    uint32_t payload_data_size;
    uint8_t ret = hd_camera_protocol_decode(protocol_data_in, protocol_data_size_in, slave_addr_out, &cmd, &payload_data_size,
                                            &payload_data);

    if (DEBUG){
        //printf("ret = %u payload_data_size=%u\n",ret,payload_data_size);
        hd_camera_protocol_print_buffer(payload_data,payload_data_size,"hd_camera_protocol_cmd_heartbeat_req_decode::result");
    }

    if (ret != 0) return -1;
    if (payload_data_size == 1) {
        *ack_number_out = payload_data[0];
        return 0;
    } else {
        return -1;
    }
}

uint8_t hd_camera_protocol_cmd_heartbeat_resp_decode(
        const unsigned char *protocol_data_in,
        uint32_t protocol_data_size_in,
        uint8_t *slave_addr_out,
        uint8_t *ack_number_out
) {
    uint8_t cmd;
    unsigned char *payload_data;
    uint32_t payload_data_size;
    uint8_t ret = hd_camera_protocol_decode(protocol_data_in, protocol_data_size_in, slave_addr_out, &cmd, &payload_data_size,
                                            &payload_data);

    if (DEBUG){
        //printf("ret = %u payload_data_size=%u\n",ret,payload_data_size);
        hd_camera_protocol_print_buffer(payload_data,payload_data_size,"hd_camera_protocol_cmd_heartbeat_resp_decode::result");
    }

    if (ret != 0) return -1;
    if (payload_data_size == 1) {
        *ack_number_out = payload_data[0];
        return 0;
    } else {
        return -1;
    }
}


uint8_t hd_camera_protocol_cmd_property_get_req(uint8_t slave_addr, uint8_t property_id) {
    return 0;
}

uint8_t
hd_camera_protocol_cmd_property_get_resp(uint8_t *slave_addr, uint32_t *len, uint8_t *property_id, uint8_t *result,
                                         unsigned char **property_value) {
    return 0;
}

uint8_t
hd_camera_protocol_cmd_property_set_req(uint8_t slave_addr, uint8_t property_id, const unsigned char *property_value,
                                        uint32_t property_value_size) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_set_resp(uint8_t *slave_addr, uint8_t *property_id, uint8_t *result) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_recovery_req(uint8_t slave_addr) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_recovery_resp(uint8_t *slave_addr, uint8_t *result) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_reboot_req(uint8_t slave_addr) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_reboot_resp(uint8_t *slave_addr, uint8_t *result) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_snapshot_req(uint8_t slave_addr) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_snapshot_resp(uint8_t *slave_addr, uint8_t *result, uint8_t *pic_id) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_info_req(uint8_t slave_addr) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_info_resp(uint8_t *slave_addr, uint32_t *len, uint8_t *pic_num,
                                                         hd_camera_protocol_pic_info **pic_infos) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_delete_req(uint8_t slave_addr, uint8_t pic_id) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_delete_resp(uint8_t *slave_addr, uint8_t *result) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_pull_req(uint8_t slave_addr, uint8_t pic_id, uint32_t offset,
                                                        uint32_t read_len) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_pull_resp(uint8_t *slave_addr, uint32_t *len, uint8_t *result,
                                                         unsigned char **pic_data) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_pull_completed_req(uint8_t slave_addr, uint8_t pic_id) {
    return 0;
}

uint8_t
hd_camera_protocol_cmd_property_camera_pull_completed_resp(uint8_t *slave_addr, uint8_t pic_id, uint8_t *result) {
    return 0;
}

uint8_t
hd_camera_protocol_cmd_property_camera_ota_req(uint8_t slave_addr, uint32_t file_size, const unsigned char *file_md5) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_ota_resp(uint8_t *slave_addr, uint8_t *result, uint32_t *offset) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_ota_push_req(uint8_t slave_addr, uint32_t offset, uint32_t data_len,
                                                            const unsigned char *data) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_ota_push_resp(uint8_t *slave_addr, uint8_t *result) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_door_signal_req(uint8_t slave_addr) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_door_signal_resp(uint8_t *slave_addr, uint8_t *result) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_req(uint8_t slave_addr, uint32_t file_size,
                                                               unsigned char *file_md5) {
    return 0;
}

uint8_t
hd_camera_protocol_cmd_property_camera_app_upgrade_resp(uint8_t *slave_addr, uint8_t *result, uint32_t *offset) {
    return 0;
}

uint8_t
hd_camera_protocol_cmd_property_camera_app_upgrade_push_req(uint8_t slave_addr, uint32_t offset, uint32_t data_len,
                                                            unsigned char *data) {
    return 0;
}

uint8_t hd_camera_protocol_cmd_property_camera_app_upgrade_push_resp(uint8_t *slave_addr, uint8_t *result) {
    return 0;
}