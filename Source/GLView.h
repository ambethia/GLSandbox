//
//  GLView.h
//  GLSandbox
//
//  Created by Jason Perry on 1/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <QuartzCore/CVDisplayLink.h>

@interface GLView : NSOpenGLView {
  CVDisplayLinkRef displayLink;
}

@end
