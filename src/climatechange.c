/* Authors: Autumn Ara
 * Description: Define how the top level drawing and simulation
 *              functions are brought together in the main function.
 *              Define a fatal error proecedure for inferior functions.
 */

#include "climatechange.h"

#include <stdlib.h>

/* Description: Main function for the final executable, made of a
 *              simulation and drawing process. The main function,
 *              initializes, loops, and cleans up both processes
 *              through their top level functions.
 * Returns: Successful exit code on successful exit
 */
int main() {
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
