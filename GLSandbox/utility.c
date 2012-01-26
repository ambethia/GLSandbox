//
//  utility.c
//  GLSandbox
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#import <CoreFoundation/CFBundle.h>

#include "sandbox.h"
#include "utility.h"

static void *file_contents(const char *filename, GLint *length)
{
  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
  char path[PATH_MAX];
  CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX);
  chdir(path);
  CFRelease(resourcesURL);
  
  FILE *f = fopen(filename, "r");
  void *buffer;
  
  if (!f) {
    fprintf(stderr, "Unable to open %s for reading\n", filename);
    return NULL;
  }
  
  fseek(f, 0, SEEK_END);
  *length = (GLint)ftell(f);
  fseek(f, 0, SEEK_SET);
  
  buffer = malloc(*length+1);
  *length = (GLint)fread(buffer, 1, *length, f);
  fclose(f);
  ((char*)buffer)[*length] = '\0';
  
  return buffer;
}

GLuint createBuffer(GLenum target, GLsizei size, const void *data, GLenum type)
{
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, type);
  glBindBuffer(target, 0);
  return buffer;
}

GLuint createShader(GLenum eShaderType  , const char *filename)
{
  GLint length;
  GLuint shader;
  GLint status;
  GLchar *source = file_contents(filename, &length);
  
  if (!source)
    return 0;
  
  shader = glCreateShader(eShaderType);
  
  glShaderSource(shader, 1, (const GLchar**)&source, &length);
  glCompileShader(shader);
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    GLchar *strInfoLog = malloc(infoLogLength);
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
    
    const char *strShaderType = NULL;
    switch(eShaderType) {
      case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
      case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }
    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    free(strInfoLog);
  }
  
  return shader;
}

// createProgram(2, GL_VERTEX_SHADER, "vertex.glsl");
// createProgram(4, GL_VERTEX_SHADER, "vertex.glsl", GL_FRAGMENT_SHADER, "fragment.glsl");
GLuint createProgram(int numArgs, ...)
{
  int numShaders = numArgs / 2;
  int i;
  GLint status;
  GLuint program;
  GLuint shaders[numShaders];
  va_list argp;

  va_start(argp, numArgs);

  program = glCreateProgram();
  
  for (i = 0; i < numShaders; i++) {
    GLenum type = va_arg(argp, GLenum);
    const char* filename = va_arg(argp, const char*);
    shaders[i] = createShader(type, filename);
    glAttachShader(program, shaders[i]);
  }
  va_end(argp);
  
  glLinkProgram(program);
  
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);    
    GLchar *strInfoLog = malloc(infoLogLength);
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Link failure in program:\n%s\n", strInfoLog);
    free(strInfoLog);
    
    return GL_FALSE;
  } else {
    for (i = 0; i < numShaders; i++) {
      glDeleteShader((GLuint)shaders[i]);
    }
  }
  
  return program;
}
