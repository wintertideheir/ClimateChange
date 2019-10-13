#include "climatechange.h"

#include <stdlib.h>

int main(int argc, char const *argv[]) {
  simulationSetup();
  drawingSetup();
  while(!exitRequested()) {
    drawingProcess();
    simulationProcess();
  }
  drawingCleanup();
  simulationCleanup();
  return EXIT_SUCCESS;
}

void fatalError() {
  drawingCleanup();
  simulationCleanup();
  exit(EXIT_FAILURE);
}
