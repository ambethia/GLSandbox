//
//  sandbox.c
//  Triangle
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <math.h>
#include <string.h>
#include "sandbox.h"

GLuint program, vertexBuffer, offsetLocation;

const GLfloat vertices[] = {
  0.0f,    0.3f, 0.0f, 1.0f,
  0.3f, -0.166f, 0.0f, 1.0f,
  -0.3f, -0.166f, 0.0f, 1.0f,
  1.0f,    0.0f, 0.0f, 1.0f,
  0.0f,    1.0f, 0.0f, 1.0f,
  0.0f,    0.0f, 1.0f, 1.0f,
};

static void adjustOffsets(float *x, float *y, GLfloat time)
{
  const float duration = 5.0f;
  const float scale = 3.14159f * 2.0f / duration;
  const float position = fmodf(time, duration);

  *x = cosf(position * scale) * 0.5f;
  *y = sinf(position * scale) * 0.5f;
}

GLboolean sandboxSetup()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  GLuint vertexShader = createShader(GL_VERTEX_SHADER, "shader.v.glsl");
  GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, "shader.f.glsl");
  
  program = createProgram(2, vertexShader, fragmentShader);
  if(!program)
    return GL_FALSE;
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  vertexBuffer = createBuffer(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertices, GL_STREAM_DRAW);
  if(!vertexBuffer)
    return GL_FALSE;

  offsetLocation = glGetUniformLocation(program, "offset");
  
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  return GL_TRUE;
}

void sandboxRender(GLfloat time)
{
  float x = 0.0f, y = 0.0f;
  adjustOffsets(&x, &y, time);

  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);

  glUniform2f(offsetLocation, x, y);
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat) * 3 * 4));

  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glUseProgram(0);
}
