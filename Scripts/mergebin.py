#!/usr/bin/env python3
#
# Copyright (C) 2023 alson <tx_danyang@163.com>
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

import os
import sys
import argparse

def paras_analysis():
    parser = argparse.ArgumentParser(description='merge bin file to txt file')
    parser.add_argument('-i', type=str, nargs='+', help='input bin file path, can be either a relative path or an absolute path')
    parser.add_argument('-o', type=str, help='output txt file name')
    parser.add_argument('-u', type=int, default=8192, help='bin alignment size, not enough to fill 0xFF')
    args = parser.parse_args()
    return args

def paras_check(bin_file_path, txt_file_name, count):
    for bin_file in bin_file_path:
        if not os.path.exists(bin_file):
            print(f'{bin_file} does not exist')
            return False

    if os.path.exists(txt_file_name):
        print(f'{txt_file_name} exists')
        return False

    if count % 4 != 0:
        print(f'[-c] must be a multiple of 4')
        return False

    return True

def bin_merge(bin_file_path, txt_file_name, count):
    bin_file_out = open(txt_file_name, 'wb')

    for bin_file in bin_file_path:
        bin_file_size = os.path.getsize(bin_file)
        bin_file_copy_size = 0
        bin_file_fill_size = 0

        if bin_file_size >= count:
            bin_file_copy_size = count
            bin_file_fill_size = 0
        else:
            bin_file_copy_size = bin_file_size
            bin_file_fill_size = count - bin_file_size

        bin_file_in  = open(bin_file, 'rb')
        data = bin_file_in.read(bin_file_copy_size)
        bin_file_out.write(data)

        if bin_file_fill_size:
            for cnt in range(0, bin_file_fill_size):
                bin_file_out.write(0xFF.to_bytes(1, byteorder='little'))

        bin_file_in.close()

    bin_file_out.close()

if __name__ == '__main__':
    args = paras_analysis()

    bin_file_path = args.i
    txt_file_name = args.o
    count = args.u

    if not paras_check(bin_file_path, txt_file_name, count):
        sys.exit(-1)

    bin_merge(bin_file_path, txt_file_name, count)

    sys.exit(0)
