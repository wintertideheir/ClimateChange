#include "climatechange.h"

#include <stdlib.h>

int main(int argc, char const *argv[]) {
  globe = regular_icosahedron(GLOBE_DETAIL);
  drawingBegin();
  while(!drawingShouldClose()) {
    drawingLoop();
  }
  drawingEnd();
  delete_mesh(globe);
  return EXIT_SUCCESS;
}

void earlyExit() {
  drawingEnd();
  delete_mesh(globe);
  exit(EXIT_FAILURE);
}
