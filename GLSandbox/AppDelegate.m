//
//  AppDelegate.m
//  GLSandbox
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

- (void)createOpenGLView;

- (void)createDisplayLink;

- (void)renderForTime:(CVTimeStamp)time;

@end

CVReturn displayCallback(CVDisplayLinkRef displayLink,
                         const CVTimeStamp *inNow,
                         const CVTimeStamp *inOutputTime,
                         CVOptionFlags flagsIn, 
                         CVOptionFlags *flagsOut,
                         void *displayLinkContext);

CVReturn displayCallback(CVDisplayLinkRef displayLink,
                         const CVTimeStamp *inNow,
                         const CVTimeStamp *inOutputTime,
                         CVOptionFlags flagsIn,
                         CVOptionFlags *flagsOut,
                         void *displayLinkContext)
{
  AppDelegate *controller = (AppDelegate *)displayLinkContext;
  [controller renderForTime:*inOutputTime];
  return kCVReturnSuccess;
}

@implementation AppDelegate
{
  CVDisplayLinkRef displayLink;
}

@synthesize window;
@synthesize view;

- (void)createOpenGLView
{
  NSOpenGLPixelFormatAttribute pixelFormatAttributes[] = {
    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
    NSOpenGLPFAColorSize,     24,
    NSOpenGLPFAAlphaSize,     8,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFANoRecovery,
    0
  };

  NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]
                                       initWithAttributes:pixelFormatAttributes] autorelease];

  [self setView:[[[NSOpenGLView alloc] initWithFrame:[[[self window] contentView] bounds]
                                         pixelFormat:pixelFormat] autorelease]];

  [[[self window] contentView] addSubview:[self view]];
  [[self view] setAutoresizingMask:(NSViewHeightSizable | NSViewWidthSizable)];
}

- (void)createDisplayLink
{
  CGDirectDisplayID displayID = CGMainDisplayID();
  CVReturn error = CVDisplayLinkCreateWithCGDisplay(displayID, &displayLink);
  
  if (kCVReturnSuccess == error) {
    CVDisplayLinkSetOutputCallback(displayLink, displayCallback, self);
    CVDisplayLinkStart(displayLink);
  }
  else {
    NSLog(@"Display Link created with error: %d", error);
    displayLink = NULL;
  }
}


- (void)renderForTime:(CVTimeStamp)time
{
  [[[self view] openGLContext] makeCurrentContext];

  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  [[[self view] openGLContext] flushBuffer];
}

- (void)awakeFromNib
{
  [self createOpenGLView];
  [self createDisplayLink];
}

- (void)dealloc
{
  [super dealloc];
}

@end
