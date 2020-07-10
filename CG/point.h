#pragma once
#include <stdlib.h>
#include "glut.h"
#include <cmath>

struct point {
	GLfloat pos[2];
	GLubyte color[3];

	point() : pos{ 0, 0 }, color{ 0,0,0 } {
	}

	point(GLfloat x, GLfloat y) : pos{ x, y }, color{ 0,0,0 } {
	}

	point(GLfloat x, GLfloat y, GLubyte r, GLubyte g, GLubyte b) : pos{ x, y }, color{r, g, b} {
	}

};
