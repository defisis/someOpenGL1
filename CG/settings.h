#pragma once
#include <stdlib.h>
#include "glut.h"

namespace settings {
	GLint width = 1200;
	GLint height = 600;
	GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
	int selectedObject = 0;
	bool isRandomColor = true;
	bool isDisplayGrid = false;
	bool isTextureEnable = false;
	bool isLogicEnable = false;
	int logicOp = 0;
	int rastrMode = 0;
	int cellSize = 12;
}