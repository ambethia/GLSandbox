#include "sandbox.h"
#include "vertices.h"

GLuint program, offsetUniLoc;

GLuint vertexBufferObject;
GLuint vertexArrayObject;

GLboolean sandboxSetup(void)
{
  glClearColor(0.2, 0.2, 0.2, 1.0);

  program = createProgram(4, GL_VERTEX_SHADER, "shader.v.glsl", GL_FRAGMENT_SHADER, "shader.f.glsl");
  if (!program)
    return GL_FALSE;

  offsetUniLoc = glGetUniformLocation(program, "offset");

  vertexBufferObject = createBuffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  if(!vertexBufferObject)
    return GL_FALSE;
  
  glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  return GL_TRUE;
}

void sandboxRender(GLfloat time)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  
	glUniform2f(offsetUniLoc, 0.5f, 0.25f);
  
	size_t colorData = sizeof(vertices) / 2;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData);
  
	glDrawArrays(GL_TRIANGLES, 0, 36);
  
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
  glUseProgram(0);
}
