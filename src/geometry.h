#include <amth.h>

float golden_ratio = (1 + sqrtf(5)) / 2;

float icosahedron_vertices[12][3] =
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
   {-golden_ratio, 0, -1}}

int icosahedron_faces[20][3] =
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
   {6, 8, 1}}
