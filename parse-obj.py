#!/usr/bin/env python

def float_to_fixed(value):
    return int(float(value * 256.0))

def line_to_coords(line):
    return [float_to_fixed(float(value)) for value in line.split(' ')[1:]]

def line_to_face(line):
    indices = [[int(index) for index in value.split('/')] for value in line.split(' ')[1:]]

    for vertex_indices in indices:
        if vertex_indices[0] != vertex_indices[2]:
            raise ValueError('Vertex coordinate index does not match vertex normal index')

    return [vertex_indices[0] - 1 for vertex_indices in indices]

def vertex_to_str(vertex, normal):
    return f'{{{{{", ".join([str(value) for value in vertex])}}}, {{0}}, {{0}}, {{{", ".join([str(value) for value in normal])}}}}}'

def face_to_str(face):
    return f'''{{(Vertex*[]) {{{", ".join([f'&_vertex_data[{index}]' for index in face])}}}, {len(face)}}}'''

filename = 'cube.obj'
vertices = []
normals = []
faces = []
for line in open(filename, 'r'):
    if line.startswith('v '):
        vertices.append(line_to_coords(line))
    elif line.startswith('vn '):
        normals.append(line_to_coords(line))
    elif line.startswith('f '):
        faces.append(line_to_face(line))

output = f'''
#include <gba_base.h>

#include "types.h"

Vertex _vertex_data[] = {{
  {', '.join([vertex_to_str(vertices[i], normals[i]) for i in range(0, len(vertices))])}
}};
Polygon _polygon_data[] = {{
  {', '.join([face_to_str(face) for face in faces])}
}};
Object object = {{_vertex_data, _polygon_data, {len(vertices)}, {len(faces)}}};
'''

print(output.strip())
