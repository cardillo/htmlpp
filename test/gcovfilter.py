#!/usr/bin/env python

import argparse
import fileinput
import re

def main(opts):
    out = False
    for line in map(str.rstrip, fileinput.input(opts.files)):
        if re.search('0:Source:', line):
            out = any(map(lambda p: re.search(p, line), opts.include))
        if out:
            print(line)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--include', action='append', default=[])
    parser.add_argument('files', nargs='*')
    opts = parser.parse_args()
    main(opts)
