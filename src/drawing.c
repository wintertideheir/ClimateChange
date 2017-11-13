#include "drawing.h"

#include <stdio.h>

int windowX = 800;
int windowY = 600;

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

  int sizex, sizey;
  glfwGetFramebufferSize(window, &sizex, &sizey);
  glViewport(0, 0, sizex, sizey);
}

void drawingLoop() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

int drawingShouldClose() {
  return glfwWindowShouldClose(window);
}

void drawingEnd() {
  glfwTerminate();
}
