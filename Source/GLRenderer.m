//
//  GLRenderer.m
//  GLSandbox
//
//  Created by Jason Perry on 1/29/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#import "Renderers.h"

@interface GLRenderer (PrivateMethods)
- (void) setup;
@end

@implementation GLRenderer

- (id) initWithDefaultFBO: (GLuint) name
{
	if((self = [super init]))
	{
		NSLog(@"%s %s", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    defaultFBOName = name;
    [self setup];
  }
  return self;
}

- (void) setup
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
}

- (void) resizeWithWidth:(GLuint)width AndHeight:(GLuint)height
{
  glViewport(0, 0, (GLsizei) width, (GLsizei) width);
}

- (void)renderWithTime:(GLfloat)time
{
  glClear(GL_COLOR_BUFFER_BIT);
}

@end
