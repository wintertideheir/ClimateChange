#include "climatechange.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

GLuint globeShaderProgram;

GLint globe_lightUniform;
GLint globe_viewUniform;
GLint globe_projectionUniform;

unsigned int globe_VBO;
unsigned int globe_EBO;
unsigned int globe_VAO;

void generateShaders();
