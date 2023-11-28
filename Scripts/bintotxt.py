#!/usr/bin/env python3
#
# Copyright (C) 2023 alson <tx_danyang@163.com>
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

import os
import sys
import argparse

def paras_analysis():
    parser = argparse.ArgumentParser(description='convert bin file to txt file')
    parser.add_argument('-i', type=str, help='input bin file path, can be either a relative path or an absolute path')
    parser.add_argument('-o', type=str, help='output txt file name')
    parser.add_argument('-c', type=int, default=16, help='number of bytes displayed per line, must be a multiple of 4')
    args = parser.parse_args()
    return args

def paras_check(bin_file_path, txt_file_name, count):
    if not os.path.exists(bin_file_path):
        print(f'{bin_file_path} does not exist')
        return False

    if os.path.exists(txt_file_name):
        print(f'{txt_file_name} exists')
        return False

    if count % 4 != 0:
        print(f'[-c] must be a multiple of 4')
        return False

    return True

def convert(bin_file_path, txt_file_name, count):
    bin_file_size = os.path.getsize(bin_file_path)
    written_bytes = 0

    bin_file = open(bin_file_path, 'rb')
    txt_file = open(txt_file_name, 'w')

    while True:
        byte = bin_file.read(1)
        txt_file.write(byte.hex().upper())

        written_bytes = written_bytes + 1
        if written_bytes >= bin_file_size:
            break
        if written_bytes % count == 0:
            txt_file.write('\r')
        else:
            txt_file.write(' ')

    txt_file.close()
    bin_file.close()

if __name__ == '__main__':
    args = paras_analysis()

    bin_file_path = args.i
    txt_file_name = args.o
    count = args.c

    if not paras_check(bin_file_path, txt_file_name, count):
        sys.exit(-1)

    convert(bin_file_path, txt_file_name, count)

    sys.exit(0)
