//
//  utility.h
//  GLSandbox
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#ifndef GLSandbox_utility_h
#define GLSandbox_utility_h

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>

GLuint createBuffer(GLenum target, GLsizei size, const void *data, GLenum type);

GLuint createShader(GLenum type, const char *filename);

GLuint createProgram(int n, ...);

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

#endif
