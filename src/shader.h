#define GLEW_STATIC
#include <GL/glew.h>

struct ShaderRequest {
  GLenum type;
  int* indicies;
  int indicies_len;
};

GLuint createShader(GLenum type, GLsizei number, const GLchar **code);
GLuint createProgram(const GLchar** shader_code, struct ShaderRequest* shader_req, int shader_req_len);
