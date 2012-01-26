//
//  utility.h
//  GLSandbox
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef GLSandbox_utility_h
#define GLSandbox_utility_h

GLuint createBuffer(GLenum target, GLsizei size, const void *data, GLenum type);

GLuint createShader(GLenum type, const char *filename);

GLuint createProgram(int n, ...);

#endif
