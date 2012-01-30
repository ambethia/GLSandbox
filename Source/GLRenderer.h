//
//  GLRenderer.h
//  GLSandbox
//
//  Created by Jason Perry on 1/29/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "utility.h"

@interface GLRenderer : NSObject {
  GLuint defaultFBOName;
}

- (id) initWithDefaultFBO: (GLuint) defaultFBOName;
- (void) resizeWithWidth:(GLuint)width AndHeight:(GLuint)height;
- (void) renderWithTime:(GLfloat)time;

@end
