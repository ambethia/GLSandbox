//
//  TriangleRenderer.m
//  GLSandbox
//
//  Created by Jason Perry on 1/29/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#import "Renderers.h"

static GLuint program, vertexBuffer, timeUniformLoc;

static const GLfloat vertices[] = {
  0.0f,    0.3f, 0.0f, 1.0f,
  0.3f, -0.166f, 0.0f, 1.0f,
  -0.3f, -0.166f, 0.0f, 1.0f,
  1.0f,    0.0f, 0.0f, 1.0f,
  0.0f,    1.0f, 0.0f, 1.0f,
  0.0f,    0.0f, 1.0f, 1.0f,
};

@implementation TriangleRenderer

- (void)setup
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  program = createProgram(4, GL_VERTEX_SHADER,   "triangle.v.glsl",
                             GL_FRAGMENT_SHADER, "triangle.f.glsl");  
  vertexBuffer = createBuffer(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertices, GL_STREAM_DRAW);
  
  timeUniformLoc = glGetUniformLocation(program, "time");
  
  GLuint vertDurationUniformLoc = glGetUniformLocation(program, "vertDuration");
  GLuint fragDurationUniformLoc = glGetUniformLocation(program, "fragDuration");
  
  glUseProgram(program);
  glUniform1f(vertDurationUniformLoc, 5.0f);
  glUniform1f(fragDurationUniformLoc, 10.0f);
  glUseProgram(0);
  
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

- (void)renderWithTime:(GLfloat)time
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

- (void)resizeWithWidth:(GLuint)width AndHeight:(GLuint)height
{
  if(width < height)
    glViewport(0, 0, (GLsizei) width, (GLsizei) width);
  else
    glViewport(0, 0, (GLsizei) height, (GLsizei) height);  
}

@end
