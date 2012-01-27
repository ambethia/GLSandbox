#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>

#include <math.h>
#include <string.h>
#include "utility.h"

int sandboxWidth;
int sandboxHeight;

GLboolean sandboxSetup(void);

void sandboxRender(GLfloat time);

void sandboxReshape(int w, int h);
