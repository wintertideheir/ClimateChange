#include "drawing.h"

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
    earlyExit();
  }

  return shader;
};

struct ShaderRequest {
  GLenum type;
  int* indicies;
  int indicies_len;
};

GLuint createProgram(const GLchar** shader_code,
                     struct ShaderRequest* shader_req, int shader_req_len) {
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

    earlyExit();
  }

  for (int i = 0; i < shader_req_len; i++) {
    glDeleteShader(shaders[i]);
  }

  return shaderProgram;
}

void generateShaders()
{
  const GLchar* globeVertexShaderCode =
  "#version 330 core\n"
  "layout (location = 0) in vec3 pos;\n"
  "uniform int windowX;\n"
  "uniform int windowY;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"
  "float near = 1;\n"
  "float far = 100;\n"
  "float right = windowX/2;\n"
  "float top = windowY/2;\n"
  "void main()\n"
  "{\n"
  "    gl_Position = projection * view * vec4(pos, 1.0);\n"
  "}\n";

  const GLchar* globeFragmentShaderCode =
  "#version 330 core\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "    color = vec4(vec3(1.0), 0.0);\n"
  "}\n";

  const GLchar* globeProgramCode[] =
    {globeVertexShaderCode, globeFragmentShaderCode};
  struct ShaderRequest globeProgramReq[] =
    {(struct ShaderRequest){GL_VERTEX_SHADER, &(int){0}, 1},
     (struct ShaderRequest){GL_FRAGMENT_SHADER, &(int){1}, 1}};
  globeShaderProgram =
    createProgram(globeProgramCode, globeProgramReq, 2);
}
