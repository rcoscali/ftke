/*
 * License Applicability. Except to the extent portions of this file are
 * made subject to an alternative license as permitted in the SGI Free
 * Software License B, Version 1.1 (the "License"), the contents of this
 * file are subject only to the provisions of the License. You may not use
 * this file except in compliance with the License. You may obtain a copy
 * of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
 * Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
 * 
 * http://oss.sgi.com/projects/FreeB
 * 
 * Note that, as provided in the License, the Software is distributed on an
 * "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
 * DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
 * CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
 * 
 * Original Code. The Original Code is: OpenGL Sample Implementation,
 * Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
 * Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
 * Copyright in any portions created by third parties is as indicated
 * elsewhere herein. All Rights Reserved.
 * 
 * Additional Notice Provisions: The application programming interfaces
 * established by SGI in conjunction with the Original Code are The
 * OpenGL(R) Graphics System: A Specification (Version 1.2.1), released
 * April 1, 1999; The OpenGL(R) Graphics System Utility Library (Version
 * 1.3), released November 4, 1998; and OpenGL(R) Graphics with the X
 * Window System(R) (Version 1.3), released October 19, 1998. This software
 * was created using the OpenGL(R) version 1.2.1 Sample Implementation
 * published by SGI, but has not been independently verified as being
 * compliant with the OpenGL(R) version 1.2.1 Specification.
 *
 */

/*
 * hello.c
 * This is a simple, introductory OpenGL program.
 */
#include "ug.h"

#define EGL_PRECISION 16					// number of fractional bits
#define EGL_ONE		  (1 << EGL_PRECISION)	// representation of 1
#define EGL_ZERO	  0						// representation of 0
#define EGL_HALF	  0x08000				// S15.16 0.5 
#define EGL_PINF	  0x7fffffff			// +inf 
#define EGL_MINF	  0x80000000			// -inf 
#define FFI(value) ((value) << EGL_PRECISION)
#define FFF(value) (value * EGL_ONE)

static void draw_line(GLfixed x1, GLfixed y1, GLfixed x2, GLfixed y2)
{
	GLfixed v4[4] = {x1, y1, x2, y2};

	glVertexPointer(2, GL_FIXED, 0, v4);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINES, 0, 2);

	return;
}

static void drawOneLine(float x1, float y1, float x2, float y2) {
   GLfloat v[4];
   v[0] = x1; v[1] = y1; v[2] = x2; v[3] = y2;
   glVertexPointer(2, GL_FLOAT, 0, v);
   glEnableClientState(GL_VERTEX_ARRAY);
   glDrawArrays(GL_LINES, 0, 2);
}

void display(UGWindow uwin)
{
	glClear (GL_COLOR_BUFFER_BIT);

	glColor4f (1.0, 1.0, 0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glColor4f (1.0, 0, 0, 1.0);
	glRotatef(90.0, 0, 0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glColor4f (0, 1.0, 0, 1.0);
	glTranslatef(10.0, 10.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glFlush ();
	ugSwapBuffers(uwin);
}

const static GLfloat v[] = 
{
	0.0,   0.0,   0.0,
	0.5,   0.0,   0.0,
	0.0,   0.5,   0.0,
	0.5,   0.5,   0.0
};

static void init (void) 
{
	int width = 0;
	int height = 0;
	EGLSurface surface = NULL;

	surface = eglGetCurrentSurface(EGL_READ);
	eglQuerySurface(eglGetCurrentDisplay(), surface, EGL_WIDTH, &width);
	eglQuerySurface(eglGetCurrentDisplay(), surface, EGL_HEIGHT, &height);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport( 0, 0, width, height);

	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState(GL_VERTEX_ARRAY);

	return;
}

/* 
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA).  Open window with "hello"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
   UGCtx ug = ugInit();
   UGWindow uwin = ugCreateWindow (ug, "", "hello", 250, 250, 100, 100);

   init();
   ugDisplayFunc(uwin, display); 
   ugMainLoop(ug);

   return 0;  
}
