cmake_minimum_required(VERSION 3.21)
project(hd_camera C)

set(CMAKE_C_STANDARD 99)

#add_executable(hd_camera_test ./test/test.c hd_camera_protocol.c hd_camera_protocol_cmd.c hd_camera_protocol_property.h)

add_executable(
        hd_uart_host
        test_hd_camera_uar_host.c
        hd_camera_protocol.c
        hd_camera_protocol_cmd.c
        hd_camera_protocol_property.c
        hd_camera_uart_host.c
        hd_uart.c
)

#add_executable(
#        hd_uart_slave
#        test_hd_camera_uart_slave.c
#        hd_camera_protocol.c
#        hd_camera_protocol_cmd.c
#        hd_camera_protocol_property.c
#        hd_camera_uart_slave.c
#        hd_uart.c
#)
