//
//  sandbox.c
//  Triangle
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include "sandbox.h"
#include "utility.h"

GLuint program;
GLuint vertexBuffer;

GLboolean sandboxSetup()
{
  const GLfloat vertices[] = {
    0.75f,  0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
  };
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  GLuint vertexShader = createShader(GL_VERTEX_SHADER, "shader.v.glsl");
  GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, "shader.f.glsl");
  
  program = createProgram(2, vertexShader, fragmentShader);
  if(!program)
    return GL_FALSE;
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  vertexBuffer = createBuffer(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
  if(!vertexBuffer)
    return GL_FALSE;

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  return GL_TRUE;
}

void sandboxRender(GLfloat time)
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  glDisableVertexAttribArray(0);
  glUseProgram(0);
}
