/* Authors: Autumn Ara
 * Description: Define how the top level drawing and simulation
 *              functions are brought together in the main function.
 *              Define a fatal error proecedure for inferior functions.
 */

#include "climatechange.h"

#include <stdlib.h>

/* Description: The main function handles initialization, looping, and
 *              cleans up through top level functions.
 * Returns: Successful exit code on successful exit
 */
int main() {
  globe = geodesic_icosahedron(GLOBE_DETAIL);
  drawingSetup();
  while(!exitRequested()) {
    drawingProcess();
  }
  drawingCleanup();
  delete_mesh(globe);
  return EXIT_SUCCESS;
}

void fatalError() {
  drawingCleanup();
  delete_mesh(globe);
  exit(EXIT_FAILURE);
}
