/* Authors: Autumn Ara
 * Description: Defines top level simulation functions.
 */

#include "climatechange.h"

void simulationSetup() {
  globe = geodesic_icosahedron(GLOBE_DETAIL);
}

void simulationProcess() {
}

void simulationCleanup() {
  delete_mesh(globe);
}
