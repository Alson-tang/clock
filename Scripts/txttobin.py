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
    parser.add_argument('-i', type=str, help='input txt file path, can be either a relative path or an absolute path')
    parser.add_argument('-o', type=str, help='output txt file name')
    args = parser.parse_args()
    return args

def paras_check(txt_file_path, bin_file_name):
    if not os.path.exists(txt_file_path):
        print(f'{txt_file_path} does not exist')
        return False

    if os.path.exists(bin_file_name):
        print(f'{bin_file_name} exists')
        return False

    return True

def convert(txt_file_path, bin_file_name):
    bin_file = open(bin_file_name, 'wb+')

    with open(txt_file_path, 'r') as txt_file:
        for line in txt_file:
            start = line.find('0X')
            end = line.find(',')
            while start != -1:
                data_str = line[start + 2: end]
                data = int(data_str,16)

                bin_file.write(data.to_bytes(1, byteorder='little'))

                start = end
                start = line.find('0X', start)
                end = line.find(',', start)

    bin_file.close()

if __name__ == '__main__':
    args = paras_analysis()

    txt_file_path = args.i
    bin_file_name = args.o

    if not paras_check(txt_file_path, bin_file_name):
        sys.exit(-1)

    convert(txt_file_path, bin_file_name)

    sys.exit(0)
