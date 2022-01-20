#!/usr/bin/env python3

# ex:
# python3 script/cpp_formatter.py -d [sub_dir]
# python3 script/cpp_formatter.py -f [file]

import argparse
import glob
import os
import subprocess
import sys

cwd = os.getcwd()

def parse_arguments(args):
    parser = argparse.ArgumentParser(description='code formatter')
    parser.add_argument('-f', '--file', default='',\
        help='Format with file')
    parser.add_argument('-d', '--directory', default='',
        help='Format with directory ')
    args = parser.parse_args()
    return args

def get_files(path):
    file_sets = ('*.c', '*.cpp', '*.hpp', '*.h')

    files = [glob.glob(path + "/" + file_name) for file_name in file_sets]
    files.sort()
    return [element for array in files for element in array]

def format_file(filename):
    print(f"Formatting file : {filename}")
    command = "clang-format --style=file -i "
    command += filename
    subprocess.call(command, shell=True)

def format_directory(path):
    print(f"Formatting directory : {path}")
    for (root, _, files) in os.walk(cwd+"/"+path):
        print("* Formatting files in " + root)
        if len(files) > 0:
            for filename in get_files(root):
                format_file(filename)

if __name__ == "__main__":
    args = parse_arguments(sys.argv)
    if args.file != '':
        format_file(args.file)
    elif args.directory != '':
        format_directory(args.directory)
    else:
        os.system("Please check your option")
