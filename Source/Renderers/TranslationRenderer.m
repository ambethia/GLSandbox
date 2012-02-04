//
//  TranslationRenderer.m
//  GLSandbox
//
//  Created by Jason Perry on 1/29/12.
//  Copyright (c) 2012 Jason L Perry. All rights reserved.
//

#import "GLRenderer.h"

#include "matrixUtil.h"

@implementation TranslationRenderer

const int numberOfVertices = 8;

#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR   1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR  0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f

const float vertexData[] =
{
	+1.0f, +1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
  
	-1.0f, -1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, +1.0f, +1.0f,
  
	GREEN_COLOR,
	BLUE_COLOR,
	RED_COLOR,
	BROWN_COLOR,
  
	GREEN_COLOR,
	BLUE_COLOR,
	RED_COLOR,
	BROWN_COLOR,
};

const GLshort indexData[] =
{
	0, 1, 2,
	1, 0, 3,
	2, 3, 0,
	3, 2, 1,
  
	5, 4, 6,
	4, 5, 7,
	7, 6, 4,
	6, 7, 5,
};

GLuint shaderProgram;

GLuint modelToCameraMatrixUnif;
GLuint cameraToClipMatrixUnif;

GLfloat cameraToClipMatrix[16];
GLfloat modelToCameraMatrix[16];

GLuint vertexBufferObject;
GLuint indexBufferObject;
GLuint vertexArrayObject;

GLfloat fieldOfView = 45.0f;
GLfloat fzNear      = 1.0f;
GLfloat fzFar       = 45.0f;
GLfloat viewRatio;

- (void)setup
{
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  glClearDepth(1.0f);

  vertexBufferObject = createBuffer(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  indexBufferObject = createBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
  
  shaderProgram = createProgram(4,
    GL_VERTEX_SHADER,   "pos_color_local_transform.v.glsl",
    GL_FRAGMENT_SHADER, "color_passthrough.f.glsl"
  );

  glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
  
  modelToCameraMatrixUnif = glGetUniformLocation(shaderProgram, "modelToCameraMatrix");
	cameraToClipMatrixUnif = glGetUniformLocation(shaderProgram, "cameraToClipMatrix");

  size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

  glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
  
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

- (void)renderWithTime:(GLfloat)time
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);
  glBindVertexArray(vertexArrayObject);

  mtxLoadPerspective(cameraToClipMatrix, fieldOfView, viewRatio, fzNear, fzFar);  
	glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, cameraToClipMatrix);

  const float fLoopDuration = 6.0f;
  const float fScale = 3.14159f * 2.0f / fLoopDuration;

  float fCurrTimeThroughLoop = fmodf(time, fLoopDuration);
  
  // Stationary
	mtxLoadTranslate(modelToCameraMatrix, 0.0f, 0.0f, -20.0f);
  glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, modelToCameraMatrix);
  glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

  // Top
  mtxLoadTranslate(modelToCameraMatrix,
                   cosf(fCurrTimeThroughLoop * fScale) * 4.f,
                   sinf(fCurrTimeThroughLoop * fScale) * 6.f,
                   -20.0f);
  glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, modelToCameraMatrix);
  glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

  // Bottom
  mtxLoadTranslate(modelToCameraMatrix,
                   cosf(fCurrTimeThroughLoop * fScale) * 5.f,
                   -3.5f,
                   sinf(fCurrTimeThroughLoop * fScale) * 5.f - 20.0f);
  glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, modelToCameraMatrix);
  
  glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

  
	glUseProgram(0);
}

- (void) resizeWithWidth:(GLuint)width AndHeight:(GLuint)height
{
  viewRatio = 1.0f; //(GLfloat)width / (GLfloat)height;
  [super resizeWithWidth:width AndHeight:height];
}

@end
