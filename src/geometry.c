#include "geometry.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct Mesh regular_icosahedron(int subdivisions) {

  #define ICOSAHEDRON_FACES(level) ((int) (10 * powf(2, level)) + 2)
  #define ICOSAHEDRON_EDGES(level) (20 * (int) powf(2, level))
  #define ICOSAHEDRON_VERTICES(level) (30 * (int) powf(2, level))

  struct Mesh icosahedron;
  icosahedron.vertices_len = ICOSAHEDRON_VERTICES(subdivisions);
  icosahedron.faces_len = ICOSAHEDRON_FACES(subdivisions);
  icosahedron.vertices = malloc(icosahedron.vertices_len);
  icosahedron.faces = malloc(icosahedron.faces_len);

  float golden_ratio = (1 + sqrtf(5)) / 2;

  float original_vertices[12][3] =
    {{0,  1,  golden_ratio},
     {0, -1,  golden_ratio},
     {0,  1, -golden_ratio},
     {0, -1, -golden_ratio},
     { 1, golden_ratio,  0},
     {-1, golden_ratio,  0},
     { 1, -golden_ratio, 0},
     {-1, -golden_ratio, 0},
     { golden_ratio, 0,  1},
     {-golden_ratio, 0,  1},
     { golden_ratio, 0, -1},
     {-golden_ratio, 0, -1}};

  int original_faces[20][3] =
    {{4, 0, 5},
     {4, 5, 2},
     {4, 2, 10},
     {4, 10, 8},
     {4, 8, 0},
     {9, 0, 5},
     {9, 5, 1},
     {9, 1, 7},
     {9, 7, 11},
     {9, 11, 0},
     {3, 11, 2},
     {3, 2, 10},
     {3, 10, 6},
     {3, 6, 7},
     {3, 7, 11},
     {11, 2, 5},
     {10, 6, 8},
     {6, 1, 7},
     {0, 1, 8},
     {6, 8, 1}};

  float vertices_1[icosahedron.vertices_len][3];
  float vertices_2[icosahedron.vertices_len][3];
  int faces_1[icosahedron.faces_len][3];
  int faces_2[icosahedron.faces_len][3];

  memcpy(vertices_1, &original_vertices, sizeof(original_vertices));
  memcpy(faces_1, &original_faces, sizeof(original_faces));

  float (*vertices_from)[icosahedron.vertices_len][3] = &vertices_1;
  float (*vertices_to)[icosahedron.vertices_len][3] = &vertices_2;
  int (*faces_from)[icosahedron.faces_len][3] = &faces_1;
  int (*faces_to)[icosahedron.faces_len][3] = &faces_2;

  int vertices_to_length = 0;
  int faces_to_length = 0;

  int edge_map_length = 0;
  int edge_map[ICOSAHEDRON_EDGES(subdivisions)][3];

  #define vertex(x, y, z) \
    (*vertices_to)[vertices_to_length][0] = x; \
    (*vertices_to)[vertices_to_length][1] = y; \
    (*vertices_to)[vertices_to_length][2] = z; \
    vertices_to_length++; \

  #define midpoint(from, to) \
    vertex(((*vertices_from)[from][0] + (*vertices_from)[to][0]) / 2, \
           ((*vertices_from)[from][1] + (*vertices_from)[to][1]) / 2, \
           ((*vertices_from)[from][2] + (*vertices_from)[to][2]) / 2); \

  #define edge(from, to, store) \
    for (int v = 0; v < edge_map_length; v++) { \
      if (edge_map[v][0] == from && \
          edge_map[v][1] == to) { \
        store = edge_map[v][2]; \
        edge_map[v][0] = edge_map[edge_map_length-1][0]; \
        edge_map[v][0] = edge_map[edge_map_length-1][0]; \
        edge_map[edge_map_length-1][0]; \
        edge_map[edge_map_length-1][1]; \
        edge_map_length--; \
        break; \
      } \
    } \
    if (store == -1) { \
      edge_map[edge_map_length][0] = from; \
      edge_map[edge_map_length][1] = to; \
      midpoint(from, to); \
      edge_map[edge_map_length][2] = vertices_to_length - 1; \
      edge_map_length++; \
    } \

  #define face(p1, p2, p3) \
    (*faces_to)[faces_to_length][0] = p1; \
    (*faces_to)[faces_to_length][1] = p2; \
    (*faces_to)[faces_to_length][2] = p3; \
    faces_to_length++; \

  for (int i = 0; i < subdivisions; i++) {
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
    int (*faces_temp)[icosahedron.faces_len][3] = faces_from;
    vertices_from = vertices_to;
    vertices_to = vertices_temp;
    faces_from = faces_to;
    faces_to = faces_temp;
    vertices_to_length = 0;
    faces_to_length = 0;
  }

  memcpy(icosahedron.vertices, vertices_to, icosahedron.vertices_len);
  memcpy(icosahedron.faces, faces_to, icosahedron.faces_len);

  #undef ICOSAHEDRON_VERTICES
  #undef ICOSAHEDRON_EDGES
  #undef ICOSAHEDRON_FACES

  return icosahedron;
}
