/* Authors: Autumn Ara
 * Description: Declare top level control functions,
 *              particularly drawing and simulation functions.
 */

#include "geometry.h"

/* Description: The resolution level of the globe mesh, or the number
 *              of subdivisions it goes through.
 * Minimum Value: 0
 * Note: Increasing this value increases computational load.
 */
#define GLOBE_DETAIL 5

/* Description: The 3D mesh of the globe for the simulation to occur
 *              on.
 */
struct Mesh globe;

void drawingSetup();
void drawingProcess();
void drawingCleanup();
void simulationSetup();
void simulationProcess();
void simulationCleanup();

int exitRequested();
/* Description: Call clean up functions and exit the program when a
 *              fatal error has occurred.
 */
void fatalError();
