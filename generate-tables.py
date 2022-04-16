#!/usr/bin/env python

from math import sin, pi

table_size = 256
cos_offset = table_size >> 2

def gba_color(brightness):
    return (brightness << 10) | (brightness << 5) | brightness

values = [int(sin(2.0 * pi / table_size * i) * 256.0) for i in range(0, table_size + cos_offset)]

output = f'''
#ifndef __TABLES_H__
#define __TABLES_H__

#include <gba_base.h>

#define TRIG_TABLE_SIZE {table_size}

s16 _table_data[] = {{ {", ".join([str(value) for value in values])} }};

s16* sin_table = _table_data;
s16* cos_table = &_table_data[{cos_offset}];
u16 palette[] = {{ {", ".join([str(gba_color(value >> 3)) for value in range(0, 256)])} }};

#endif
'''

print(output.strip())
