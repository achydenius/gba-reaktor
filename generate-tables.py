#!/usr/bin/env python3

from math import sin, pi, pow

# Trig table parameters
table_size = 1024

# Palette parameters
palette_size = 256
ambient = [0, 0, 0.25]
diffuse = [0, 0.25, 1.0]
specular = [1.0, 1.0, 1.0]
shininess = 2

def clamp(color):
    return max(0, min(color, 1.0))

def add_colors(a, b):
    return [clamp(x + y) for (x, y) in zip(a, b)]

def multiply_color(color, value):
    return [c * value for c in color]

def float_to_fixed(value):
    return int(value * 31)

def gba_format(color):
    return ((float_to_fixed(color[2]) << 10) |
            (float_to_fixed(color[1]) << 5) |
            float_to_fixed(color[0]))

# Generate palette
# ambient + diffuse * angle + specular * angle^shininess
palette_values = []
for i in range(0, palette_size):
    angle = 1.0 / palette_size * i
    diff = multiply_color(diffuse, angle)
    spec = multiply_color(specular, pow(angle, shininess))
    palette_values.append(add_colors(ambient, add_colors(diff, spec)))

# Generate sin/cos data
cos_offset = table_size >> 2
trig_values = [int(sin(2.0 * pi / table_size * i) * 256.0) for i in range(0, table_size + cos_offset)]

output = f'''
#ifndef __TABLES_H__
#define __TABLES_H__

#include <gba_base.h>

#define TRIG_TABLE_SIZE {table_size}

s16 _table_data[] = {{{', '.join([str(value) for value in trig_values])}}};

s16* sin_table = _table_data;
s16* cos_table = &_table_data[{cos_offset}];
u16 palette[] = {{{','.join([str(gba_format(value)) for value in palette_values])}}};
#endif
'''

print(output.strip())
