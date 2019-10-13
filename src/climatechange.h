#include "geometry.h"

#define GLOBE_DETAIL 5

struct Mesh globe;

void drawingSetup();
void drawingProcess();
void drawingCleanup();
void simulationSetup();
void simulationProcess();
void simulationCleanup();

int exitRequested();
void fatalError();
