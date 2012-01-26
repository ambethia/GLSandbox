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

GLuint program, vertexBuffer, timeUniformLoc;

const GLfloat vertices[] = {
  0.0f,    0.3f, 0.0f, 1.0f,
  0.3f, -0.166f, 0.0f, 1.0f,
  -0.3f, -0.166f, 0.0f, 1.0f,
  1.0f,    0.0f, 0.0f, 1.0f,
  0.0f,    1.0f, 0.0f, 1.0f,
  0.0f,    0.0f, 1.0f, 1.0f,
};

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

  timeUniformLoc = glGetUniformLocation(program, "time");
  
  GLuint durationUniformLoc = glGetUniformLocation(program, "duration");
  glUseProgram(program);
  glUniform1f(durationUniformLoc, 5.0f);
  glUseProgram(0);
  
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  return GL_TRUE;
}

void sandboxRender(GLfloat time)
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);

  glUniform1f(timeUniformLoc, time);
  
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
