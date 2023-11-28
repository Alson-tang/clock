#!/usr/bin/env python3
#
# Copyright (C) 2023 alson <tx_danyang@163.com>
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

import os
import sys
import argparse
import serial
import serial.tools.list_ports
import enum
import tqdm

class ymodem_file_type(enum.Enum):
    YMODEM_FILE_INFO = 0
    YMODEM_FILE_DATA = 1
    YMODEM_FILE_END = 2

YMODEM_SOH      = 0x01
YMODEM_STX      = 0X02
YMODEM_EOT      = 0x04
YMODEM_ACK      = 0x06
YMODEM_NAK      = 0x15
YMODEM_CAN      = 0x18
YMODEM_C        = 0x43
YMODEM_CPMEOF   = 0x1A

frame_128_head_index = 0
frame_128_blk_index = 1
frame_128_inverse_blk_index = 2
frame_128_data_index = 3
frame_128_crc_high = 131
frame_128_crc_low = 132

frame_1024_head_index = 0
frame_1024_blk_index = 1
frame_1024_inverse_blk_index = 2
frame_1024_data_index = 3
frame_1024_crc_high = 1027
frame_1024_crc_low = 1028

frame_blk = 0

crctable = [
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
]

def paras_analysis():
    parser = argparse.ArgumentParser(description='send Ymodem')
    parser.add_argument('-p', type=str, default='COM0', help='serial port number')
    parser.add_argument('-s', type=int, choices=[128, 1024], default=128, help='package size, can only be 128 or 1024. unit: byte')
    parser.add_argument('-f', type=str, help='file path, can be either a relative path or an absolute path')
    parser.add_argument('-addr', type=int, default=0, help='flash start address')
    args = parser.parse_args()
    return args

def paras_check(serial_port, file_path):
    port_list = []
    for port in serial.tools.list_ports.comports():
        port_list.append(port.name)
    if serial_port not in port_list:
        print(f'{serial_port} does not exist')
        return False

    if not os.path.exists(file_path):
        print(f'{file_path} does not exist')
        return False

    return True

def ymodem_crc16(ch, crc = 0):
    crctbl_idx = ((crc >> 8) ^ ch) & 0xff
    crc = ((crc << 8) ^ crctable[crctbl_idx]) & 0xffff

    return crc & 0xFFFF

def ymodem_send_and_recv(ser, frame, expected_value, c_required = False):
    ser.flushInput()

    if ser.write(frame) != len(frame):
        print(f'The length to be sent does not match the actual length sent')
        return False

    ch_1 = ser.read()
    ch_1_dig = int.from_bytes(ch_1, byteorder='big', signed=False)
    while ch_1_dig == YMODEM_C:
        ch_1 = ser.read()
        ch_1_dig = int.from_bytes(ch_1, byteorder='big', signed=False)

    if ch_1_dig == expected_value:
        if c_required:
            ch_2 = ser.read()
            ch_2_dig = int.from_bytes(ch_2, byteorder='big', signed=False)
            if ch_2_dig != YMODEM_C:
                print(f'The second char is not 0x43(\'C\')')
                return False
            else:
                return True
        else:
            return True
    else:
        if ch_1_dig == YMODEM_CAN:
            ch_2 = ser.read()
            ch_2_dig = int.from_bytes(ch_2, byteorder='big', signed=False)
            if ch_2_dig == YMODEM_CAN:
                print(f'The peer terminated the transmission')

                can_array = bytearray([YMODEM_CAN,YMODEM_CAN])

                ser.write(can_array)
                return False
            else:
                print(f'Failed to receive two consecutive 0x18(\'CAN\')')
                return False
        else:
            print(f'Received incorrect response')
            return False

def ymodem_frame_init(pkg_size, pkg_total_size, file_type, start_addr, file_path = None, file_size = 0, file_data_start_add = 0):
    frame = bytearray(pkg_total_size)

    global frame_blk

    file_name_index = 0
    data_index = 0
    crc_high_index = 0
    crc_low_index = 0
    blk_index = 0
    inverse_blk_index = 0

    if pkg_size == 128:
        file_name_index = frame_128_data_index
        data_index = frame_128_data_index
        crc_high_index = frame_128_crc_high
        crc_low_index = frame_128_crc_low
        blk_index = frame_128_blk_index
        inverse_blk_index = frame_128_inverse_blk_index

        frame[frame_128_head_index] = YMODEM_SOH
        frame[blk_index] = frame_blk
        frame[inverse_blk_index] = 0xFF - frame[frame_128_blk_index]
    else:
        file_name_index = frame_1024_data_index
        data_index = frame_1024_data_index
        crc_high_index = frame_1024_crc_high
        crc_low_index = frame_1024_crc_low
        blk_index = frame_1024_blk_index
        inverse_blk_index = frame_1024_inverse_blk_index

        frame[frame_1024_head_index] = YMODEM_STX
        frame[blk_index] = frame_blk
        frame[inverse_blk_index] = 0xFF - frame[frame_1024_blk_index]

    if file_type == ymodem_file_type.YMODEM_FILE_INFO:
        file_name_str = os.path.basename(file_path)
        file_size_str = hex(file_size)[2:]

        file_size_index = file_name_index + len(file_name_str) + 1

        for i in range(0, len(file_name_str)):
            frame[file_name_index + i] = int(hex(ord(file_name_str[i])), 16)

        frame[file_size_index] = 0x30
        frame[file_size_index + 1] = 0x78

        for i in range(0, len(file_size_str)):
            frame[file_size_index + 2 + i] = int(hex(ord(file_size_str[i])), 16)

        start_addr_index = file_size_index + len(file_size_str) + 2 + 1
        start_addr_str = hex(start_addr)
        for i in range(0, len(start_addr_str)):
            frame[start_addr_index + i] = int(hex(ord(start_addr_str[i])), 16)
        
    elif file_type == ymodem_file_type.YMODEM_FILE_DATA:
        if file_data_start_add + pkg_size > file_size:
            pkg_act_size = file_size - file_data_start_add
        else:
            pkg_act_size = pkg_size

        with open(file_path, 'rb') as file:
            file.seek(file_data_start_add, 0)
            fs = file.read(pkg_act_size)
            # print(fs.hex(' '))
            for i in range(pkg_act_size):
                frame[data_index + i] = fs[i]

        if pkg_act_size != pkg_size:
            for i in range(pkg_size - pkg_act_size):
                frame[data_index + pkg_act_size + i] = YMODEM_CPMEOF

        frame[blk_index] = frame_blk
        frame[inverse_blk_index] = 0xFF - frame[blk_index]
    else:
        frame[blk_index] = 0x00
        frame[inverse_blk_index] = 0xFF - frame[blk_index]

    crc = 0
    for i in range(pkg_size):
        crc = ymodem_crc16(frame[data_index + i], crc)

    frame[crc_high_index] = (crc >> 8) & 0xFF
    frame[crc_low_index] = crc & 0xFF

    # print(frame.hex())

    frame_blk = (frame_blk + 1) & 0xFF

    return frame

def ymodem_send_file_info(ser, pkg_size, pkg_total_size, file_path, start_addr):
    file_size = os.path.getsize(file_path)

    frame = ymodem_frame_init(pkg_size,
                              pkg_total_size,
                              ymodem_file_type.YMODEM_FILE_INFO,
                              start_addr,
                              file_path,
                              file_size)

    return ymodem_send_and_recv(ser, frame, YMODEM_ACK, True)

def ymodem_send_file_data(ser, pkg_size, pkg_total_size, file_path, start_addr):
    file_size = os.path.getsize(file_path)
    file_data_start_addr = 0
    ret = True

    process_bar = tqdm.tqdm(total=file_size, desc='Progress', leave=True, unit='B', unit_scale=True)

    while (ret):
        # print(f'file_data_start_addr is {file_data_start_addr}')

        frame = ymodem_frame_init(pkg_size,
                                pkg_total_size,
                                ymodem_file_type.YMODEM_FILE_DATA,
                                start_addr,
                                file_path,
                                file_size,
                                file_data_start_addr)

        ret = ymodem_send_and_recv(ser, frame, YMODEM_ACK, False)

        if ret:
            if file_data_start_addr + pkg_size >= file_size:
                return True
            else:
                file_data_start_addr += pkg_size

        process_bar.update(pkg_size)

    process_bar.close()
    return ret

def ymodem_send_file_complete_request(ser):
    cmd = bytearray(1)

    cmd[0] = YMODEM_EOT
    if not ymodem_send_and_recv(ser, cmd, YMODEM_NAK, False):
        return False

    cmd[0] = YMODEM_EOT
    if not ymodem_send_and_recv(ser, cmd, YMODEM_ACK, True):
        return False

    return True

def ymodem_send_file_end(ser, pkg_size, pkg_total_size):
    frame = ymodem_frame_init(pkg_size,
                              pkg_total_size,
                              ymodem_file_type.YMODEM_FILE_END,
                              start_addr)

    return ymodem_send_and_recv(ser, frame, YMODEM_ACK, False)

def ymodem_send_handle(ser, pkg_size, file_path, start_addr):
    pkg_total_size = 0
    if pkg_size == 128:
        pkg_total_size = 1 + 1 + 1 + 128 + 2    # head(1B) + BLK(1B) + (255 - BLK)(1B) + pkg_size(256B) + CRC(2B)
    else:
        pkg_total_size = 1 + 1 + 1 + 1024 + 2    # head(1B) + BLK(1B) + (255 - BLK)(1B) + pkg_size(1024B) + CRC(2B)

    if not ymodem_send_file_info(ser, pkg_size, pkg_total_size, file_path, start_addr):
        return False

    if not ymodem_send_file_data(ser, pkg_size, pkg_total_size, file_path, start_addr):
        return False

    if not ymodem_send_file_complete_request(ser):
        return False

    if not ymodem_send_file_end(ser, pkg_size, pkg_total_size):
        return False

    return True

def ymodem_send(serial_port, pkg_size, file_path, start_addr):
    ser = serial.Serial(port = serial_port,
                        baudrate = 115200,
                        bytesize = serial.EIGHTBITS,
                        parity = serial.PARITY_NONE,
                        stopbits  = serial.STOPBITS_ONE)

    if ser.isOpen():
        print(f'{serial_port} open successfully')
    else:
        print(f'{serial_port} open failed')
        return False

    ymodem_send_handle(ser, pkg_size, file_path, start_addr)

    print(f'{serial_port} close')
    ser.close()

if __name__ == '__main__':
    args = paras_analysis()

    serial_port = args.p
    pkg_size = args.s
    file_path = args.f
    start_addr = args.addr

    if not paras_check(serial_port, file_path):
        sys.exit(-1)

    ymodem_send(serial_port, pkg_size, file_path, start_addr)

    sys.exit(0)
