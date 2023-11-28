#!/usr/bin/env python3
#
# Copyright (C) 2023 alson <tx_danyang@163.com>
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

import os
import sys
import argparse
import struct
import random

def paras_analysis():
    parser = argparse.ArgumentParser(description='generate a bin file of the specified size')
    parser.add_argument('-of', type=str, default='random.bin', help='output file name')
    parser.add_argument('-unit', type=str, default='B', help='unit, can only be B or KB or MB')
    parser.add_argument('-count', type=int, default=1, help='copy only N input blocks')
    parser.add_argument('-value', type=str, help='fill data. eg 0xFF')
    args = parser.parse_args()
    return args

def args_valid_check(file_name, unit, count):
    if unit != 'B' and unit != 'KB' and unit != 'MB':
        print(f"-unit parameter invalid")
        return False

    return True

def out_file_check(file_name):
    return os.path.exists(file_name)

def out_file_size(unit, count):
    unit_map = { 'B' : 1, 'KB' : 1024, 'MB' : 1024 * 1024}
    return unit_map.get(unit, 0) * count

def out_file_generate(file_name, file_size, is_random, fill_value):
    file = open(file_name, 'wb')

    if is_random:
        for i in range(0, file_size):
            random_byte = struct.pack('B', random.randint(0, 255))
            file.write(random_byte)
    else:
        for i in range(0, file_size):
            file.write(fill_value.to_bytes(1, byteorder='little'))

    file.close()

    return

if __name__ == '__main__':
    args = paras_analysis()

    out_file_name = args.of
    size_unit = args.unit
    size_count = args.count

    is_random = True
    fill_value = 0xFF
    value = args.value
    if not value == None:
        is_random = False
        fill_value = int(value,16)

    if not args_valid_check(out_file_name, size_unit, size_count):
        sys.exit(-1)

    file_status = out_file_check(out_file_name)
    if file_status == True:
        print(f"{out_file_name} already exists")
        sys.exit(0)

    out_file_size = out_file_size(size_unit, size_count)

    out_file_generate(out_file_name, out_file_size, is_random, fill_value)

    sys.exit(0)
