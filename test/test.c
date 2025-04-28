#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../hd_camera_protocol_cmd.h"
#include "../hd_camera_protocol_property.c"

static void test_001_hd_camera_protocol_encode_hd_camera_protocol_decode(){
    printf("====test_001_hd_camera_protocol_encode_hd_camera_protocol_decode====\n");
    printf("----hd_camera_protocol_encode-----\n");
    unsigned char *frame;
    uint32_t frame_size;
    unsigned char debug_payload[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t slave_addr = PROTOCOL_SLAVE_ADDR_DEFAULT;

    // 生成数据帧 (命令=0x02, 从机地址=0x55)
    int ret = hd_camera_protocol_encode(&frame, &frame_size, 0x02, slave_addr, sizeof(debug_payload), debug_payload);
    if (ret != 0) {
        printf("生成失败, 错误码: %d\n", ret);
        return ;
    }

    // 打印生成的数据
    printf("生成的数据帧 (小端模式):\n");
    for (uint32_t i = 0; i < frame_size; i++) {
        printf("%02X ", frame[i]);
    }
    printf("\n");



    printf("----hd_camera_protocol_parse-----\n");
    uint8_t cmd, *payload,slave_addr_decode;
    uint32_t payload_size;

    int result = hd_camera_protocol_decode(frame,frame_size,
                                           &slave_addr_decode, &cmd, &payload_size, &payload);

    if (result == 0) {
        printf("解析成功:\n");
        printf("从机地址: 0x%02X\n", slave_addr_decode);
        printf("命令: 0x%02X\n", cmd);
        printf("数据长度: %u\n", payload_size);
        printf("数据: ");
        for (uint32_t i = 0; i < payload_size; i++) {
            printf("%02X ", payload[i]);
        }
        printf("\n");
    } else {
        printf("解析失败，错误码: %d\n", result);
    }


    // 释放内存
    free(frame);
    printf("====test_001_hd_camera_protocol_encode_hd_camera_protocol_decode==== success\n");
}

static void test_002_hd_camera_protocol_cmd_heartbeat(){
    printf("====test_002_hd_camera_protocol_cmd_heartbeat====\n");
    uint8_t  ret;
    printf("\n----hd_camera_protocol_cmd_heartbeat_req_encode-----\n");
    unsigned  char * protocol_data;
    uint32_t  protocol_data_size;
    int index = 11;
    ret = hd_camera_protocol_cmd_heartbeat_req_encode(&protocol_data, &protocol_data_size, CMD_HEARTBEAT,index);
    printf("hd_camera_protocol_cmd_heartbeat_req_encode ret=%u\n",ret);

    printf("\n----hd_camera_protocol_cmd_heartbeat_req_decode-----\n");
    uint8_t slave_addr_decode_req;
    uint8_t ack_number_decode_req;
    ret = hd_camera_protocol_cmd_heartbeat_req_decode(protocol_data,protocol_data_size,&slave_addr_decode_req,&ack_number_decode_req);
    printf("hd_camera_protocol_cmd_heartbeat_req_decode ret=%u\n",ret);
    if (ret==0){
        printf("hd_camera_protocol_cmd_heartbeat_req_decode slave_addr_decode=%u \n",slave_addr_decode_req);
        printf("hd_camera_protocol_cmd_heartbeat_req_decode ack_number_decode=%u \n",ack_number_decode_req);
    }
    if (ack_number_decode_req != index){
        printf("hd_camera_protocol_cmd_heartbeat_req_decode error index!\n");
        exit(-1);
    }
    if (slave_addr_decode_req != CMD_HEARTBEAT){
        printf("hd_camera_protocol_cmd_heartbeat_req_decode error slave_addr!\n");
        exit(-1);
    }

    printf("\n----hd_camera_protocol_cmd_heartbeat_resp_encode-----\n");
    unsigned  char * resp_protocol_data;
    uint32_t  resp_protocol_data_size;
    ret = hd_camera_protocol_cmd_heartbeat_resp_encode(&resp_protocol_data, &resp_protocol_data_size,CMD_HEARTBEAT, index+1);
    printf("hd_camera_protocol_cmd_heartbeat_resp_encode ret=%u\n",ret);

    printf("\n----hd_camera_protocol_cmd_heartbeat_resp_decode-----\n");
    uint8_t slave_addr_decode;
    uint8_t ack_number_decode;
    ret = hd_camera_protocol_cmd_heartbeat_resp_decode(resp_protocol_data,resp_protocol_data_size,&slave_addr_decode,&ack_number_decode);
    printf("hd_camera_protocol_cmd_heartbeat_resp_decode ret=%u\n",ret);
    if (ret==0){
        printf("hd_camera_protocol_cmd_heartbeat_resp_decode slave_addr_decode=%u \n",slave_addr_decode);
        printf("hd_camera_protocol_cmd_heartbeat_resp_decode ack_number_decode=%u \n",ack_number_decode);
    }
    free(resp_protocol_data);
    free(protocol_data);
    if (ack_number_decode != index+1){
        printf("hd_camera_protocol_cmd_heartbeat_resp_decode error index!\n");
        exit(-1);
    }
    if (slave_addr_decode != CMD_HEARTBEAT){
        printf("hd_camera_protocol_cmd_heartbeat_resp_decode error slave_addr!\n");
        exit(-1);
    }
    printf("====test_002_hd_camera_protocol_cmd_heartbeat==== success!\n");

}

int main(int argc ,char  *argv[]) {
    test_001_hd_camera_protocol_encode_hd_camera_protocol_decode();
    test_002_hd_camera_protocol_cmd_heartbeat();
    printf("end.\n");
    return 0;
}