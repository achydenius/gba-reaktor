#!/usr/bin/env python3
from math import sqrt

def vector_subtract(a, b):
    return [a[0] - b[0], a[1] - b[1], a[2] - b[2]]


def vector_length(v):
    return sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]))


def vector_cross(a, b, c):
    u = vector_subtract(b, a)
    v = vector_subtract(c, a)

    return [
        (u[1] * v[2]) - (u[2] * v[1]),
        (u[2] * v[0]) - (u[0] * v[2]),
        (u[0] * v[1]) - (u[1] * v[0])
    ]

def calculate_normal(p1, p2, p3):
    normal = vector_cross(p1, p2, p3)
    length = vector_length(normal)
    return [value / length for value in normal]

def float_to_fixed(value):
    return str(int(float(value * 256.0)))

def vector_to_str(vector):
    return f'{{{", ".join([float_to_fixed(value) for value in vector])}}}'

def face_to_str(face, normal):
    return f'''{{(Vertex*[]){{{', '.join([f'&_vertex_data[{index}]' for index in face])}}}, {len(face)}, {vector_to_str(normal)}, 0}}'''

filename = 'monogram.obj'
vertices = []
faces = []
normals = []
for line in open(filename, 'r'):
    if line.startswith('v '):
        vertices.append([float(value) for value in line.split(' ')[1:]])
    elif line.startswith('f '):
        faces.append([int(value.split('/')[0]) - 1 for value in line.split(' ')[1:]])

for face in faces:
    normals.append(calculate_normal(
        vertices[face[0]], vertices[face[1]], vertices[face[2]]
    ))

output = f'''
#include <gba_base.h>

#include "types.h"

Vertex _vertex_data[] = {{
  {', '.join(['{{{}}}'.format(vector_to_str(vertex)) for vertex in vertices])}
}};
Polygon _polygon_data[] = {{
  {', '.join([face_to_str(faces[i], normals[i]) for i in range(0, len(faces))])}
}};
Object object = {{_vertex_data, _polygon_data, {len(vertices)}, {len(faces)}}};
'''

print(output.strip())
