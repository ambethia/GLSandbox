#include "sandbox.h"

GLboolean sandboxSetup(void)
{
  glClearColor(0.2, 0.2, 0.2, 1.0);
  return GL_TRUE;
}

void sandboxRender(GLfloat time)
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void sandboxReshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);  
}