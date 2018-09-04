#include "drawing.h"

#define M_PI 3.14159
#define M_PI_4 (M_PI / 4)
#define M_PI_2 (M_PI / 2)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>

int windowX = 800;
int windowY = 600;
float viewf = 0.1; //Move 1 degree for every 10 pixels
float viewX = 0;
float viewY = 0;
double lastX = 0;
double lastY = 0;
enum ControlState {CONTROL_OBJECTS, CONTROL_VIEW} controlState = CONTROL_OBJECTS;
vec3 light = {0, 0, 10};

mat4 view, projection;

void framebufferSizeCallback(GLFWwindow *w, int x, int y)
{
  windowX = x;
  windowY = y;
  glm_perspective(glm_rad(60), ((float) x) / ((float) y), 0.1, 100, projection);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_projectionUniform, 1,
                            GL_FALSE, (float*) projection);
  glViewport(0, 0, x, y);
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
  viewX = fmod((viewX + (viewf * (lastX - x))), 360);
  viewY = fmin(fmax(viewY + (viewf * (lastY - y)), -90), 90);
  lastX = x;
  lastY = y;
  vec3 viewVec = {0, 0, 5};
  glm_vec_rotate(viewVec, glm_rad(viewX), (vec3){0, 1, 0});
  glm_vec_rotate(viewVec, glm_rad(viewY), (vec3){1, 0, 0});
  glm_lookat(viewVec, (vec3){0, 0, 0}, (vec3){0, 1, 0}, view);
  glProgramUniformMatrix4fv(globeShaderProgram, globe_viewUniform, 1,
                            GL_FALSE, (float*) view);
}

void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
  {
    if (controlState == CONTROL_VIEW)
    {
      glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetCursorPosCallback(w, NULL);
      controlState = CONTROL_OBJECTS;
    }
    else
    {
      double x;
      double y;
      glfwGetCursorPos(w, &x, &y);
      lastX = x;
      lastY = y;
      glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwSetCursorPosCallback(w, cursorPosCallback);
      controlState = CONTROL_VIEW;
    }
  }
}

void drawingBegin() {
  if(!glfwInit())
  {
    fprintf(stderr, "GLFW failed to initialize\n");
    earlyExit();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  window = glfwCreateWindow(windowX, windowY, "Climate Change", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "GLFW failed to create new window\n");
    earlyExit();
  }

  glfwSetWindowSizeLimits(window, windowX, windowY, GLFW_DONT_CARE, GLFW_DONT_CARE);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum glewStatus = glewInit();
  if (glewStatus != GLEW_OK)
  {
    fprintf(stderr, "GLEW failed to initialize: %s\n",
            glewGetErrorString(glewStatus));
    earlyExit();
  }

  glfwSetKeyCallback(window, keyCallback);

  generateShaders();

  glm_lookat((vec3){0, 0, 5}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, view);
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

  float globe_mesh[globe.faces_len][3][3];
  for (int face = 0; face < globe.faces_len; face++)
  {
    for (int i = 0; i < 3; i++)
    {
      for (int k = 0; k < 3; k++)
      {
        globe_mesh[face][i][k] = globe.vertices[globe.faces[face][i]][k];
      }
    }
  }

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

void drawingLoop() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(globeShaderProgram);
  glBindVertexArray(globe_VAO);
  glDrawElements(GL_TRIANGLES, globe.faces_len * 3, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

int drawingShouldClose() {
  return glfwWindowShouldClose(window);
}

void drawingEnd() {
  glfwTerminate();
}
