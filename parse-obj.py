#!/usr/bin/env python

def float_to_fixed(value):
    return int(float(value * 256.0))

def vertex_to_str(vertex):
    return f'{{{{{", ".join([str(value) for value in vertex])}}}}}'

def face_to_str(face):
    return f'''{{(Vertex*[]) {{{", ".join([f'&_vertex_data[{index}]' for index in face])}}}, {len(face)}}}'''

filename = 'plane.obj'
vertices = []
faces = []
for line in open(filename, 'r'):
    if line.startswith('v '):
        vertices.append([float_to_fixed(float(value)) for value in line.split(' ')[1:]])
    elif line.startswith('f '):
        faces.append([int(value.split('/')[0]) - 1 for value in line.split(' ')[1:]])

output = f'''
#include <gba_base.h>

#include "types.h"

Vertex _vertex_data[] = {{
  {", ".join([vertex_to_str(vertex) for vertex in vertices])}
}};
Polygon _polygon_data[] = {{
  {", ".join([face_to_str(face) for face in faces])}
}};
Object object = {{_vertex_data, _polygon_data, {len(vertices)}, {len(faces)}}};
'''

print(output.strip())
