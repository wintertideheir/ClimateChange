/* Authors: Autumn Ara
 * Description: Defines top level drawing functons.
 */

#include "climatechange.h"

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <cglm/cglm.h>

#include "shader.h"
#include <GLFW/glfw3.h>

#define CAMERA_DISTANCE_MAX 3
#define CAMERA_DISTANCE_MIN 1.2

int windowX = 800;
int windowY = 600;
double mouseX = 0;
double mouseY = 0;

float viewf = 0.1; // Move 1 degree for every 10 pixels the mouse moves
float globeRotationY = 0;
float globeRotationX = 0;
float cameraDistance = 3;

enum ControlState {
  CONTROL_OBJECTS, // Mouse movement controls objects
  CONTROL_VIEW     // Mouse movement controls the view
} controlState = CONTROL_OBJECTS;

vec3 light = {0, 0, 10};
mat4 view, projection;

GLFWwindow* window;

GLuint globeShaderProgram;
GLint globe_lightUniform;
GLint globe_viewUniform;
GLint globe_projectionUniform;
unsigned int globe_VBO;
unsigned int globe_EBO;
unsigned int globe_VAO;

/* Description: Callback function for GLFW to properly resize the
 *              window.
 */
void framebufferSizeCallback(GLFWwindow *w, int x, int y)
{
  windowX = x;
  windowY = y;
  glm_perspective(glm_rad(60), ((float) x) / ((float) y), 0.1, 100, projection);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_projectionUniform, 1,
                            GL_FALSE, (float*) projection);
  glViewport(0, 0, x, y);
}

/* Description: Callback function for GLFW that adjusts the rotation of
 *              the camera around the globe according to mouse movement
 * Note: Only active when controlState == CONTROL_VIEW
 */
void cursorPosCallback(GLFWwindow* window, double x, double y)
{
  globeRotationY = fmod((globeRotationY + (viewf * (mouseX - x) * ((90 - fabs(globeRotationX)) / 90))), 360);
  globeRotationX = fmin(fmax(globeRotationX + (viewf * (mouseY - y)), -80), 80);
  mouseX = x;
  mouseY = y;
  vec3 viewVec = {0, 0, cameraDistance};
  glm_vec3_rotate(viewVec, glm_rad(globeRotationX), (vec3){1, 0, 0});
  glm_vec3_rotate(viewVec, glm_rad(globeRotationY), (vec3){0, 1, 0});
  glm_lookat(viewVec, (vec3){0, 0, 0}, (vec3){0, 1, 0}, view);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_viewUniform, 1,
                            GL_FALSE, (float*) view);
}

/* Description: Callback function for GLFW that adjusts the distance of
 *              the camera according to mouse wheel movement (or the
 *              equivalent thereof).
 * Note: Only active when controlState == CONTROL_VIEW
 */
void scrollCallback(GLFWwindow* window, double x, double y)
{
  cameraDistance = fmax(fmin((float) (cameraDistance - (y * 0.1)), CAMERA_DISTANCE_MAX), CAMERA_DISTANCE_MIN);
  vec3 viewVec = {0, 0, cameraDistance};
  glm_vec3_rotate(viewVec, glm_rad(globeRotationY), (vec3){0, 1, 0});
  glm_vec3_rotate(viewVec, glm_rad(globeRotationX), (vec3){1, 0, 0});
  glm_lookat(viewVec, (vec3){0, 0, 0}, (vec3){0, 1, 0}, view);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_viewUniform, 1,
                            GL_FALSE, (float*) view);
}

/* Description: Callback function for GLFW that switches between
 *              control states when the TAB key is pressed.
 */
void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
  {
    if (controlState == CONTROL_VIEW)
    {
      glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetCursorPosCallback(w, NULL);
      glfwSetScrollCallback(w, NULL);
      controlState = CONTROL_OBJECTS;
    }
    else
    {
      glfwGetCursorPos(w, &mouseX, &mouseY);
      glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPosCallback(w, cursorPosCallback);
      glfwSetScrollCallback(w, scrollCallback);
      controlState = CONTROL_VIEW;
    }
  }
}

/* Description: Setup GLFW and OpenGL to render the simulation
 */ 
void drawingSetup() {
  if(!glfwInit())
  {
    fprintf(stderr, "GLFW failed to initialize\n");
    fatalError();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  window = glfwCreateWindow(windowX, windowY, "Climate Change", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "GLFW failed to create new window\n");
    fatalError();
  }

  glfwSetWindowSizeLimits(window, windowX, windowY, GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum glewStatus = glewInit();
  if (glewStatus != GLEW_OK)
  {
    fprintf(stderr, "GLEW failed to initialize: %s\n",
            glewGetErrorString(glewStatus));
    fatalError();
  }

  glEnable(GL_DEPTH_TEST);

  glfwSetKeyCallback(window, keyCallback);

  const GLchar* globeVertexShaderCode =
  "#version 330 core\n"
  "layout (location = 0) in vec3 pos;\n"
  "out vec3 outPos;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "void main()\n"
  "{\n"
  "    outPos = pos;\n"
  "    gl_Position = projection * view * vec4(pos, 1.0);\n"
  "}\n";

  const GLchar* globeFragmentShaderCode =
  "#version 330 core\n"
  "in vec3 outPos;\n"
  "out vec4 color;\n"
  "uniform vec3 light;\n"
  "void main()\n"
  "{\n"
  "    float ambient = 0.1;\n"
  "    float diffuse = max(dot(outPos, normalize(light - outPos)), 0.0);\n"
  "    float brightness = ambient + diffuse;\n"
  "    color = vec4(vec3(brightness), 0.0);\n"
  "}\n";

  const GLchar* globeProgramCode[] =
    {globeVertexShaderCode, globeFragmentShaderCode};
  struct ShaderRequest globeProgramReq[] =
    {(struct ShaderRequest){GL_VERTEX_SHADER, &(int){0}, 1},
     (struct ShaderRequest){GL_FRAGMENT_SHADER, &(int){1}, 1}};
  globeShaderProgram =
    createProgram(globeProgramCode, globeProgramReq, 2);

  glm_lookat((vec3){0, 0, 3}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, view);
  glm_perspective(glm_rad(60), ((float) windowX) / ((float) windowY), 0.1, 100,
                  projection);

  globe_lightUniform = glGetUniformLocation(globeShaderProgram, "light");
  globe_viewUniform = glGetUniformLocation(globeShaderProgram, "view");
  globe_projectionUniform = glGetUniformLocation(globeShaderProgram, "projection");

  glProgramUniform3f(globeShaderProgram, globe_lightUniform,
                     light[0], light[1], light[2]);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_viewUniform, 1,
                            GL_FALSE, (float*) view);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_projectionUniform, 1,
                            GL_FALSE, (float*) projection);

  int sizex, sizey;
  glfwGetFramebufferSize(window, &sizex, &sizey);
  glViewport(0, 0, sizex, sizey);
  glfwSetFramebufferSizeCallback(window, &framebufferSizeCallback);

  glGenVertexArrays(1, &globe_VAO);
  glGenBuffers(1, &globe_VBO);
  glGenBuffers(1, &globe_EBO);
  glBindVertexArray(globe_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, globe_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globe_EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * globe.vertices_len * 3, globe.vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * globe.faces_len * 3, globe.faces, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

/* Description: Redraw the screen with OpenGL and poll for events with
 *              GLFW.
 */
void drawingProcess() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(globeShaderProgram);
  glBindVertexArray(globe_VAO);
  glDrawElements(GL_TRIANGLES, globe.faces_len * 3, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

/* Description: Determine whether the program has been asked to close
 *              based on GLFW.
 */
int exitRequested() {
  return glfwWindowShouldClose(window);
}

void drawingCleanup() {
  glfwTerminate();
}
