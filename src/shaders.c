/* Authors: Autumn Ara
 * Description: Defines shader manipulation functions.
 */

#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLuint createShader(GLenum type, GLsizei number, const GLchar **code)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, number, code, NULL);
  glCompileShader(shader);

  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE)
  {
    GLint size = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
    GLchar info[size];
    glGetShaderInfoLog(shader, size, NULL, info);
    fprintf(stderr, "OpenGL shader failed to compile\n%s", info);

	  glDeleteShader(shader);
    fatalError();
  }

  return shader;
};

GLuint createProgram(const GLchar** shader_code, struct ShaderRequest* shader_req, int shader_req_len)
{
  GLuint* shaders = malloc(sizeof(GLuint) * shader_req_len);

  GLuint shaderProgram = glCreateProgram();

  for (int i = 0; i < shader_req_len; i++) {
    const GLchar** code = malloc(sizeof(GLchar*) * shader_req[i].indicies_len);
    for (int j = 0; j < shader_req[i].indicies_len; j++) {
      code[j] = shader_code[shader_req[i].indicies[j]];
    }
    shaders[i] = createShader(shader_req[i].type, shader_req[i].indicies_len,
                              code);
    glAttachShader(shaderProgram, shaders[i]);
  }

  glLinkProgram(shaderProgram);

  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    GLint size = 0;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &size);
    GLchar infoLog[size];
    glGetProgramInfoLog(shaderProgram, size, NULL, infoLog);
    fprintf(stderr, "OpenGL program failed to link\n%s", infoLog);

    fatalError();
  }

  for (int i = 0; i < shader_req_len; i++) {
    glDeleteShader(shaders[i]);
  }

  return shaderProgram;
}
