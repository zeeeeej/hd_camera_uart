#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>
#include "hd_camera_uart_host.h"
#include "hd_camera_protocol.h"
#include "hd_camera_protocol_cmd.h"
#include "hd_uart.h"

static int g_fd = -1;
static int g_running = 0;
static uint8_t g_heartbeat_ack = 0;

static int opt_write(const unsigned char *data, uint32_t data_size) {
    if (g_fd < 0)return -1;
    ssize_t ret = write(g_fd, data, data_size);
    printf("[uart]opt_write:%zu\n", ret);
    return 0;
}

static void send_heartbeat() {
    g_heartbeat_ack = (g_heartbeat_ack + 1) & 0xff;
    //g_heartbeat_ack = (g_heartbeat_ack + 1) % 0x100;

    unsigned char *protocol_data_out;
    uint32_t protocol_data_size_out;
    uint8_t slave_addr_in = PROTOCOL_SLAVE_ADDR_DEFAULT;

    uint8_t ack_number_in = g_heartbeat_ack;
    uint8_t ret = hd_camera_protocol_cmd_heartbeat_req_encode(&protocol_data_out, &protocol_data_size_out,
                                                              slave_addr_in, ack_number_in);
    if (ret == 0) {
        opt_write(protocol_data_out, protocol_data_size_out);
        free(protocol_data_out);
    } else {
        printf("[uart]send_heartbeat fail.\n");
    }
}

static void *notify_thread_function(void *arg) {
    printf("[uart]notify_thread_function start.\n");
    unsigned char data_buffer[2048] = {0};
    int len;
    uint8_t ret;
    while (g_running) {
        if (g_fd < 0)break;
        ssize_t read_len = read(g_fd, &data_buffer, sizeof(data_buffer));
        len = (int) read_len;
        if (len > 0) {
            uint8_t cmd;
            uint8_t slave_addr_out;
            uint32_t payload_data_size_out;
            unsigned char *payload_data_out;
            ret = hd_camera_protocol_decode(data_buffer,
                                            (uint32_t) len,
                                            &slave_addr_out,
                                            &cmd,
                                            &payload_data_size_out,
                                            &payload_data_out);

            if (ret == 0) {
                if (cmd == CMD_HEARTBEAT) {
                    // 处理心跳应答
                    uint8_t ack_number_out;
                    ret = hd_camera_protocol_cmd_heartbeat_resp_decode(
                            payload_data_out, payload_data_size_out, &ack_number_out
                    );
                    if (ret == 0) {
                        if (ack_number_out == g_heartbeat_ack + 1) {
                            printf("[uart]heartbeat decode success.\n");
                            sleep(HEARTBEAT_INTERVAL);
                            send_heartbeat();
                        } else {
                            printf("[uart]heartbeat ack fail. %u != %u\n", ack_number_out, g_heartbeat_ack);
                        }
                    } else {
                        printf("[uart]heartbeat decode fail. ret=%u\n", ret);
                    }
                } else if (cmd == CMD_PROPERTY_GET) {
                    // 处理查询属应答
                    // hd_camera_protocol_cmd_property_get_resp_encode()
                } else {
                    printf("[uart]support cmd %u\n", cmd);
                }
                //free(payload_data_out);
            } else {
                printf("[uart]Failed to read data or invalid data format\n");
            }
        } else {
            printf("[uart]Failed to read data\n");
        }
    }
    printf("[uart]notify_thread_function end.\n");
    return NULL;
}

int hd_camera_uart_host_init(const char *path) {
    printf("[uart]hd_camera_uart_host_init %s!\n", path);
    if (path == NULL) {
        printf("[uart]path is NULL.");
        return -1;
    }

    int fd = hd_uart_open(path);
    if (fd == -1) {
        printf("[uart]uart_open err!\n");
        return -1;
    }
    int ret = hd_uart_set_opt(fd, PROTOCOL_RATE_DEFAULT, 8, 'N', 1);
    if (ret) {
        printf("[uart]set port err!\n");
        return -1;
    }

    printf("[uart]start!\n");

    g_fd = fd;
    g_running = 1;
    send_heartbeat();
    pthread_t notify_thread_t;
    pthread_create(&notify_thread_t, NULL, notify_thread_function, NULL);

    pthread_join(notify_thread_t, NULL);
    return 0;
}

void hd_camera_uart_host_deinit() {
    if (g_fd >= 0) {
        g_running = 0;
        close(g_fd);
        g_fd = -1;
    }
}



