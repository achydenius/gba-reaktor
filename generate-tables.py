#!/usr/bin/env python

from math import sin, pi

table_size = 256
cos_offset = table_size >> 2

values = [int(sin(2.0 * pi / table_size * i) * 256.0) for i in range(0, table_size + cos_offset)]

output = f'''
#ifndef __TABLES_H__
#define __TABLES_H__

#include <gba_base.h>

#define TRIG_TABLE_SIZE {table_size}

static s16 table_data[] = {{ {", ".join([str(value) for value in values])} }};

s16* sin_table = table_data;
s16* cos_table = &table_data[{cos_offset}];

#endif
'''

print(output.strip())
