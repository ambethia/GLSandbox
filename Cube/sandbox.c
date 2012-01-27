#include "sandbox.h"
#include "vertices.h"

GLuint program, offsetUniLoc, perspectiveMatrixUniLoc;

GLfloat frustumScale = 1.0f;

float perspectiveMatrix[16];

GLuint vertexBufferObject;
GLuint vertexArrayObject;

GLboolean sandboxSetup(void)
{
  glClearColor(0.2, 0.2, 0.2, 1.0);

  program = createProgram(4, GL_VERTEX_SHADER, "shader.v.glsl", GL_FRAGMENT_SHADER, "shader.f.glsl");
  if (!program)
    return GL_FALSE;

  offsetUniLoc = glGetUniformLocation(program, "offset");
  perspectiveMatrixUniLoc = glGetUniformLocation(program, "perspectiveMatrix");
  
  memset(perspectiveMatrix, 0, sizeof(float) * 16);
  
  float zNear = 0.5f;
  float zFar = 3.0f;
  
  perspectiveMatrix[0] = frustumScale;
  perspectiveMatrix[5] = frustumScale;
  perspectiveMatrix[10] = (zFar + zNear) / (zNear - zFar);
  perspectiveMatrix[14] = (2 * zFar * zNear) / (zNear - zFar);
  perspectiveMatrix[11] = -1.0f;
  
  glUseProgram(program);
  glUniformMatrix4fv(perspectiveMatrixUniLoc, 1, GL_FALSE, perspectiveMatrix);
  glUseProgram(0);

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
  
	glUniform2f(offsetUniLoc, 0.5f, 0.5f);
  
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

void sandboxReshape(int w, int h)
{
  perspectiveMatrix[0] = frustumScale / (w / (float)h);
  perspectiveMatrix[5] = frustumScale;

  glUseProgram(program);
  glUniformMatrix4fv(perspectiveMatrixUniLoc, 1, GL_FALSE, perspectiveMatrix);
  glUseProgram(0);

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

