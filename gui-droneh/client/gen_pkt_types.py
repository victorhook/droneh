#!/usr/bin/env python3

import re
import clipboard as c
import io
import sys

if __name__ == '__main__':
    out = io.StringIO()
    old = sys.stdout
    sys.stdout = out

    path = '/home/victor/coding/projects/droneh/firmware-droneh/include/protocol.h'
    with open(path) as f:
        data = f.read()
        k = re.search(r'typedef enum {(.*)} packet_type_e;', data, flags=re.DOTALL)
        data = [a.strip()  for a in k.group(1).split(',')]
        for i, d in enumerate(filter(len, data)):
            print(f'    {d} = {i}')

        print('\n    _STRINGS = {')
        for i, d in enumerate(filter(len, data)):
            print(f'        {i}: "{d}",')
        print('}')

    out.seek(0)
    old.write(out.read())
    out.seek(0)
    c.copy(out.read())