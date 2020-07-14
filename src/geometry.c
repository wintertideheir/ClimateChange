/* Authors: Autumn Ara
 * Description: Defines geometric manipulaton functions.
 */

#include "geometry.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

struct Mesh geodesic_icosahedron(int subdivisions) {

  #define ICOSAHEDRON_FACES(level) ((unsigned int) (20 * powf(4, level)))
  #define ICOSAHEDRON_EDGES(level) ((unsigned int) (30 * powf(4, level)))
  #define ICOSAHEDRON_VERTICES(level) ((unsigned int) (2 + (10 * powf(4, level))))

  struct Mesh icosahedron;
  icosahedron.vertices_len = ICOSAHEDRON_VERTICES(subdivisions);
  icosahedron.faces_len = ICOSAHEDRON_FACES(subdivisions);
  icosahedron.vertices = malloc(sizeof(float) * 3 * icosahedron.vertices_len);
  icosahedron.faces = malloc(sizeof(unsigned int) * 3 * icosahedron.faces_len);

  float original_vertices[12][3] =
    {{ 0.000000, -1.000000,  0.000000},
     { 0.723600, -0.447215,  0.525720},
     {-0.276385, -0.447215,  0.850640},
     {-0.894425, -0.447215,  0.000000},
     {-0.276385, -0.447215, -0.850640},
     { 0.723600, -0.447215, -0.525720},
     { 0.276385,  0.447215,  0.850640},
     {-0.723600,  0.447215,  0.525720},
     {-0.723600,  0.447215, -0.525720},
     { 0.276385,  0.447215, -0.850640},
     { 0.894425,  0.447215,  0.000000},
     { 0.000000,  1.000000,  0.000000}};

  unsigned int original_faces[20][3] =
    {{1,  0,  5},
     {0,  1,  2},
     {0,  2,  3},
     {0,  3,  4},
     {0,  4,  5},
     {1,  5,  10},
     {2,  1,  6},
     {3,  2,  7},
     {4,  3,  8},
     {5,  4,  9},
     {1,  10, 6},
     {2,  6,  7},
     {3,  7,  8},
     {4,  8,  9},
     {5,  9, 10},
     {6,  10, 11},
     {7,  6,  11},
     {8,  7,  11},
     {9, 8,  11},
     {10, 9, 11}};

  float vertices_1[icosahedron.vertices_len][3];
  float vertices_2[icosahedron.vertices_len][3];
  unsigned int faces_1[icosahedron.faces_len][3];
  unsigned int faces_2[icosahedron.faces_len][3];

  memcpy(vertices_1, &original_vertices, sizeof(original_vertices));
  memcpy(faces_1, &original_faces, sizeof(original_faces));

  float (*vertices_from)[icosahedron.vertices_len][3] = &vertices_1;
  float (*vertices_to)[icosahedron.vertices_len][3] = &vertices_2;
  unsigned int (*faces_from)[icosahedron.faces_len][3] = &faces_1;
  unsigned int (*faces_to)[icosahedron.faces_len][3] = &faces_2;

  int vertices_to_length = 0;
  int faces_to_length = 0;

  int edge_map_length = 0;
  unsigned int edge_map[ICOSAHEDRON_EDGES(subdivisions)][3];

  #define vertex(x, y, z) \
    (*vertices_to)[vertices_to_length][0] = x; \
    (*vertices_to)[vertices_to_length][1] = y; \
    (*vertices_to)[vertices_to_length][2] = z; \
    vertices_to_length++; \

  #define midpoint(from, to) \
    float x = ((*vertices_from)[from][0] + (*vertices_from)[to][0]) / 2; \
    float y = ((*vertices_from)[from][1] + (*vertices_from)[to][1]) / 2; \
    float z = ((*vertices_from)[from][2] + (*vertices_from)[to][2]) / 2; \
    float length = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2)); \
    vertex(x / length, y / length, z / length);

  #define edge(from, to, store) \
    store = -1; \
    for (int s = 0; s < edge_map_length; s++) { \
      if ((edge_map[s][0] == from && \
           edge_map[s][1] == to) || \
          (edge_map[s][0] == to && \
           edge_map[s][1] == from)) { \
        store = edge_map[s][2]; \
        edge_map[s][0] = edge_map[edge_map_length-1][0]; \
        edge_map[s][1] = edge_map[edge_map_length-1][1]; \
        edge_map[s][2] = edge_map[edge_map_length-1][2]; \
        edge_map_length--; \
        break; \
      } \
    } \
    if (store == -1) { \
      edge_map[edge_map_length][0] = from; \
      edge_map[edge_map_length][1] = to; \
      midpoint(from, to); \
      store = edge_map[edge_map_length][2] = vertices_to_length - 1; \
      edge_map_length++; \
    } \

  #define face(p1, p2, p3) \
    (*faces_to)[faces_to_length][0] = p1; \
    (*faces_to)[faces_to_length][1] = p2; \
    (*faces_to)[faces_to_length][2] = p3; \
    faces_to_length++; \

  #define old_vertex(v) \
    vertex((*vertices_from)[v][0], \
           (*vertices_from)[v][1], \
           (*vertices_from)[v][2]); \

  for (int i = 0; i < subdivisions; i++) {
    for (int v = 0; v < ICOSAHEDRON_VERTICES(i); v++) {
        vertex((*vertices_from)[v][0], (*vertices_from)[v][1],
               (*vertices_from)[v][2]);
    }
    for (int f = 0; f < ICOSAHEDRON_FACES(i); f++) {
      int mid1, mid2, mid3;
      edge((*faces_from)[f][0], (*faces_from)[f][1], mid1);
      edge((*faces_from)[f][1], (*faces_from)[f][2], mid2);
      edge((*faces_from)[f][2], (*faces_from)[f][0], mid3);
      face(mid1, mid2, mid3);
      face(mid1, (*faces_from)[f][1], mid2);
      face(mid2, (*faces_from)[f][2], mid3);
      face(mid3, (*faces_from)[f][0], mid1);
    }

    float (*vertices_temp)[icosahedron.vertices_len][3] = vertices_from;
    unsigned int (*faces_temp)[icosahedron.faces_len][3] = faces_from;
    vertices_from = vertices_to;
    vertices_to = vertices_temp;
    faces_from = faces_to;
    faces_to = faces_temp;
    vertices_to_length = 0;
    faces_to_length = 0;
  }

  #undef vertex
  #undef midpoint
  #undef edge
  #undef face

  memcpy(icosahedron.vertices, vertices_from, sizeof(float) * 3 * icosahedron.vertices_len);
  memcpy(icosahedron.faces, faces_from, sizeof(unsigned int) * 3 * icosahedron.faces_len);

  #undef ICOSAHEDRON_VERTICES
  #undef ICOSAHEDRON_EDGES
  #undef ICOSAHEDRON_FACES

  return icosahedron;
}

void delete_mesh(struct Mesh mesh) {
  free(mesh.vertices);
  free(mesh.faces);
}
