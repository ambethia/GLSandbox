//
//  AppDelegate.m
//  GLSandbox
//
//  Created by Jason L Perry on 1/25/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#import "AppDelegate.h"

#include "sandbox.h"

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
  [[self window] setDelegate:self];
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

  GLfloat timeValue = (GLfloat)(time.videoTime) / (GLfloat)(time.videoTimeScale);

  sandboxRender(timeValue);

  [[[self view] openGLContext] flushBuffer];
}

- (void)reshape
{
  NSSize size = [[self window] frame].size;
  [[[self view] openGLContext] makeCurrentContext];
  sandboxReshape(size.width, size.height);
}

- (void)awakeFromNib
{ 
  [self createOpenGLView];
  [self createDisplayLink];
  [[[self view] openGLContext] makeCurrentContext];
  
  if (sandboxSetup() != GL_TRUE) {
    NSLog(@"Fail.");
  }
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
  if (!sandboxWidth)
    sandboxWidth = 500;
  
  if (!sandboxHeight)
    sandboxHeight = 500;

  CGFloat x = (window.screen.frame.size.width - sandboxWidth) / 2;
  CGFloat y = (window.screen.frame.size.height - sandboxHeight) / 2;  
  NSRect frame = NSMakeRect(x, y, sandboxWidth, sandboxHeight);
  [[self window] setFrame:frame display:YES];
}

- (void)windowDidResize:(NSNotification *)notification
{
  [self reshape];
}

- (void)windowWillStartLiveResize:(NSNotification *)notification
{
  [self reshape];
}

- (void)dealloc
{
  [super dealloc];
}

@end
