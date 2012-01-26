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

GLuint createBuffer(GLenum target, const void *data, GLsizei size)
{
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, GL_STATIC_DRAW);
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

// createProgram(1, shader)
// createProgram(2, shader1, shader2)
GLuint createProgram(int n, ...)
{
  GLint status;
  GLuint program = glCreateProgram();
  va_list argp;
  
  va_start(argp, n);
  for (; n; n--)
    glAttachShader(program, va_arg(argp, GLuint));
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
  }
  return program;
}
