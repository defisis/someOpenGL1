#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "glut.h"
#include <vector>
#include <array>
#include <iostream>
#include "settings.h"
#include "constants.h"
#include "poligon.h"
#include <set>

using namespace constants;
using namespace settings;

std::vector<Poligon> objects;
std::vector<point> texCoord{ point(0.2928932188134524, 0), point(0.7071067811865474, 0), point(1, 0.2928932188134524), point(1, 0.7071067811865474),
point(0.7071067811865474, 1), point(0.2928932188134524, 1), point(0, 0.7071067811865474), point(0, 0.2928932188134524) };

bool isCenter = true;

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void refreshTitle() {
	char v[90]; sprintf_s(v, "Текущий цвет точки: R=%.3d G=%.3d B=%.3d. Номер выбранного примитива: %d.", ColorR, ColorG, ColorB, selectedObject + 1);
	glutSetWindowTitle(v);
}

void drawPoint(int x, int y, float intens, std::vector<point> &bras, std::vector<float> &intensity, bool steep) {
	if (steep) {
		point p(x, y);
		if (bras.size() == 0 || bras[bras.size() - 1].pos[0] != p.pos[0] || bras[bras.size() - 1].pos[1] != p.pos[1]) {
			bras.push_back(p);
			intensity.push_back(intens);
		}
	}
	else {
		point p(y, x);
		if (bras.size() == 0 || bras[bras.size() - 1].pos[0] != p.pos[0] || bras[bras.size() - 1].pos[1] != p.pos[1]) {
			bras.push_back(p);
			intensity.push_back(intens);
		}
	}
}

void Display(void)
{
	glClearColor(1, 1, 1, 1); glClear(GL_COLOR_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);

	int size = objects.size();
	if (!isCenter) size--;
	if (settings::isDisplayGrid) {
		glLineWidth(1);
		glBegin(GL_LINES);
		for (int i = 0; i < height + cellSize + 1; i += cellSize) {
			glColor3ub(150, 150, 150);
			glVertex2i(0, i);
			glColor3ub(150, 150, 150);
			glVertex2i(width, i);
		}
		for (int i = 0; i < width + cellSize + 1; i += cellSize) {
			glColor3ub(150, 150, 150);
			glVertex2i(i, 0);
			glColor3ub(150, 150, 150);
			glVertex2i(i, height);
		}
		glEnd();
		for (int i = 0; i < size; i++) {
			point center = objects[i].getCenter();
			glColor3ub(center.color[0], center.color[1], center.color[2]);
			std::vector<point> points = objects[i].getVertices();
			if (rastrMode == 0 || rastrMode == 1) {
				if (rastrMode == 1) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glVertexPointer(2, GL_FLOAT, sizeof(point), points.data()->pos);
					glDrawArrays(GL_POLYGON, 0, points.size());
				}
				std::vector<point> bras;
				int lx = 0, ly = 0;
				for (int k = 0; k < poligonSize; k++) {
					int x1 = points[k].pos[0];
					int y1 = points[k].pos[1];
					int x2 = points[(k + 1) % poligonSize].pos[0];
					int y2 = points[(k + 1) % poligonSize].pos[1];
					int deltaX = abs(x2 - x1);
					int deltaY = abs(y2 - y1);
					int signX = x1 < x2 ? 1 : -1;
					int signY = y1 < y2 ? 1 : -1;
					int error = deltaX - deltaY;

					lx = x2 - (x2 % cellSize) * sgn(x2);
					ly = y2 - (y2 % cellSize) * sgn(y2);

					point pt(lx, ly);
					bras.push_back(pt);

					while (x1 != x2 || y1 != y2) {
						lx = x1 - (x1 % cellSize) * sgn(x1);
						ly = y1 - (y1 % cellSize) * sgn(y1);

						int error2 = error * 2;
						if (error2 > -deltaY) {
							error -= deltaY;
							x1 += signX;
						}

						if (error2 < deltaX) {
							error += deltaX;
							y1 += signY;
						}
						if (lx < 0 || lx > width || ly < 0 || ly > height) continue;
						pt.pos[0] = lx;
						pt.pos[1] = ly;
						if (bras[bras.size() - 1].pos[0] != pt.pos[0] || bras[bras.size() - 1].pos[1] != pt.pos[1])
							bras.push_back(pt);
					}
				}
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				for (point p : bras) {
					glBegin(GL_QUADS);
						glVertex2f(p.pos[0], p.pos[1]);
						glVertex2f(p.pos[0], p.pos[1] + cellSize);
						glVertex2f(p.pos[0] + cellSize, p.pos[1] + cellSize);
						glVertex2f(p.pos[0] + cellSize, p.pos[1]);
					glEnd();
				}
			}
			if (rastrMode == 2) {
				std::vector<point> bras;
				std::vector<float> intensity;
				int lx, ly;
				for (int k = 0; k < poligonSize; k++) {
					int x1 = points[k].pos[0];
					int y1 = points[k].pos[1];
					int x2 = points[(k + 1) % poligonSize].pos[0];
					int y2 = points[(k + 1) % poligonSize].pos[1];
					bool steep = abs(y2 - y1) > abs(x2 - x1);
					if (steep) {
						std::swap(x1, y1);
						std::swap(x2, y2);
					}
					if (x1 > x2) {
						std::swap(x1, x2);
						std::swap(y1, y2);
					}
					lx = x1 - (x1 % cellSize);
					ly = y1 - (y1 % cellSize);
					drawPoint(lx, ly, 1, bras, intensity, steep);
					lx = x2 - (x2 % cellSize);
					ly = y2 - (y2 % cellSize);
					drawPoint(lx, ly, 1, bras, intensity, steep);
					float dx = x2 - x1;
					float dy = y2 - y1;
					float grad = dy / dx;
					float y = y1 + grad;
					for (int x = x1 + 1; x < x2 - 1; x++) {
						lx = x - (x % cellSize);
						ly = (int)y - ((int)y % cellSize);
						drawPoint(lx, ly, 1 - (y - (int)y), bras, intensity, steep);
						drawPoint(lx, ly + 1, y - (int)y, bras, intensity, steep);
						y += grad;
					}
				}
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				
				for (int i = 0; i < bras.size(); i++) {
					glColor3ub(center.color[0] * intensity[i], center.color[1] * intensity[i], center.color[2] * intensity[i]);
					point p = bras[i];
					glBegin(GL_QUADS);
						glVertex2f(p.pos[0], p.pos[1]);
						glVertex2f(p.pos[0], p.pos[1] + cellSize);
						glVertex2f(p.pos[0] + cellSize, p.pos[1] + cellSize);
						glVertex2f(p.pos[0] + cellSize, p.pos[1]);
					glEnd();
				}
			}
		}
		for (int i = 0; i < size; i++) {
			glLineWidth(2);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			point center = objects[i].getCenter();
			glColor3ub(255 - center.color[0], 255 - center.color[1], 255 - center.color[2]);
			std::vector<point> points = objects[i].getVertices();
			glVertexPointer(2, GL_FLOAT, sizeof(point), points.data()->pos);
			glDrawArrays(GL_POLYGON, 0, points.size());
		}

	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (isTextureEnable) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			GLubyte data54[54];
			FILE *file = fopen("zeder.bmp", "rb");
			fread(data54, 54, 1, file);
			int width = *(data54 + 18);
			int height = *(data54 + 22);
			GLubyte *pixels = new GLubyte[width * height * 3];
			fread(pixels, width * height * 3, 1, file);
			fclose(file);
			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
			glEnable(GL_TEXTURE_2D);
		}
		if (isLogicEnable) {
			glEnable(GL_COLOR_LOGIC_OP);
			if (logicOp == 0) glLogicOp(GL_AND);
			if (logicOp == 1) glLogicOp(GL_NAND);
		}
		for (int i = 0; i < size; i++) {
			std::vector<point> obj = objects[i].getVertices();
			point center = objects[i].getCenter();
			glColor3ub(center.color[0], center.color[1], center.color[2]);
			glPushMatrix();
			point move = objects[i].getMove();
			glTranslatef(center.pos[0] + move.pos[0], center.pos[1] + move.pos[1], 0);
			glRotatef(objects[i].getAngle(), 0, 0, 1);
			float scale = objects[i].getScale();
			glScalef(scale, scale, 1);
			glTranslatef(-center.pos[0] - move.pos[0], -center.pos[1] - move.pos[1], 0);
			glTranslatef(move.pos[0], move.pos[1], 0);
			if (isTextureEnable) 
				glTexCoordPointer(2, GL_FLOAT, sizeof(point), texCoord.data()->pos);
			glVertexPointer(2, GL_FLOAT, sizeof(point), obj.data()->pos);
			glDrawArrays(GL_POLYGON, 0, obj.size());
			if (i == selectedObject) {

			}
			glPopMatrix();
		}
	}
	if (isTextureEnable) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
	if (isLogicEnable) 
		glDisable(GL_COLOR_LOGIC_OP);
	glDisableClientState(GL_VERTEX_ARRAY);
	glFinish();
}

void Reshape(GLint w, GLint h)
{
	settings::width = w;    settings::height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{

	if (key == 'q') objects[selectedObject].rotate(-5);
	if (key == 'e') objects[selectedObject].rotate(5);

	if (key == 'z') objects[selectedObject].scale(0.1);
	if (key == 'x') objects[selectedObject].scale(-0.1);

	if (key == 'w') objects[selectedObject].move(0, 5);
	if (key == 's') objects[selectedObject].move(0, -5);
	if (key == 'a') objects[selectedObject].move(-5, 0);
	if (key == 'd') objects[selectedObject].move(5, 0);
	if (key == 'm') {
		if (objects.size() > 1)
			objects.erase(objects.begin() + selectedObject);
		else objects.clear();
		if (selectedObject != 0) selectedObject--;
	}

	if (key == ',') if (selectedObject != 0) 
		selectedObject--;
	if (key == '.') 
		if (selectedObject != objects.size() - 1) selectedObject++;
	if (key == '[') if (cellSize != 1)
		cellSize--;
	if (key == ']') cellSize++;
	glutPostRedisplay();
	refreshTitle();
}


void Mouse(int button, int state, int x, int y)
{

	if (state != GLUT_DOWN) return;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (isRandomColor) {
			ColorR = rand() % 250;  ColorG = rand() % 250; ColorB = rand() % 250;
		}
		if (isCenter) {
			point t(x, height - y, ColorR, ColorG, ColorB);
			objects.push_back(Poligon(t, t));
			isCenter = !isCenter;
		}
		else {
			objects[objects.size() - 1].setFirstVertex(point(x, height - y, ColorR, ColorG, ColorB));
			isCenter = !isCenter;
			refreshTitle();
		}
	}
	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (!objects.empty())
			objects.pop_back();
	}


	glutPostRedisplay();
}

void Menu(int pos)
{
	int key;
	key = (keys)pos;

	switch (key)
	{
	case KeyUp: Keyboard('w', 0, 0); break;
	case KeyDown: Keyboard('s', 0, 0); break;
	case KeyLeft: Keyboard('a', 0, 0); break;
	case KeyRight: Keyboard('d', 0, 0); break;
	case KeyRotateLeft: Keyboard('q', 0, 0); break;
	case KeyRotateRight: Keyboard('e', 0, 0); break;
	case KeyScaleUp: Keyboard('z', 0, 0); break;
	case KeyScaleDown: Keyboard('x', 0, 0); break;
	case KeyDeleteObj: Keyboard('m', 0, 0); break;
	case KeyPrevObj: Keyboard(',', 0, 0); break;
	case KeyNextObj: Keyboard('.', 0, 0); break;
	case KeyLogic: {
		logicOp = ++logicOp % 2;
		glutPostRedisplay();
		break;
	}
	case KeyEnableLogic: {
		isLogicEnable = !isLogicEnable;
		glutPostRedisplay();
		break;
	}
	case KeyEnableTexture: {
		isTextureEnable = !isTextureEnable;
		glutPostRedisplay();
		break;
	}
	case KeyDisplayGrid: {
		isDisplayGrid = !isDisplayGrid;
		glutPostRedisplay();
		break;
	}
	case KeyRastrMode: {
		rastrMode = ++rastrMode % 3;
		glutPostRedisplay();
		break;
	}
	case KeyCellSizeInc: Keyboard(']', 0, 0); break;
	case KeyCellSizeDec: Keyboard('[', 0, 0); break;
	case KeyRandomColor: {
		isRandomColor = !isRandomColor;
		break;
	}
	default:
		int menu_color = glutCreateMenu(Menu);

			glutAddMenuEntry("Randomize colors", KeyRandomColor);
		int menu_move = glutCreateMenu(Menu);
			glutAddMenuEntry("Up", KeyUp);
			glutAddMenuEntry("Down", KeyDown);
			glutAddMenuEntry("Left", KeyLeft);
			glutAddMenuEntry("Right", KeyRight);
			glutAddMenuEntry("Rotate left", KeyRotateLeft);
			glutAddMenuEntry("Rotate right", KeyRotateRight);
			glutAddMenuEntry("Scale up", KeyScaleUp);
			glutAddMenuEntry("Scale down", KeyScaleDown);
		int menu_object = glutCreateMenu(Menu);
			glutAddMenuEntry("Delete object", KeyDeleteObj);
			glutAddMenuEntry("Previous object", KeyPrevObj);
			glutAddMenuEntry("Next object", KeyNextObj);
			glutAddMenuEntry("Apply a texture", KeyEnableTexture);
			glutAddMenuEntry("Enable logic operations", KeyEnableLogic);
			glutAddMenuEntry("Switch logic operation", KeyLogic);
		int menu_grid = glutCreateMenu(Menu);
			glutAddMenuEntry("Display grid", KeyDisplayGrid);
			glutAddMenuEntry("Change rastr mode", KeyRastrMode);
			glutAddMenuEntry("Increase cell size", KeyCellSizeInc);
			glutAddMenuEntry("Decrease cell size", KeyCellSizeDec);
		int menu = glutCreateMenu(Menu);
			glutAddSubMenu("Change color", menu_color);
			glutAddSubMenu("Move", menu_move);
			glutAddSubMenu("Object", menu_object);
			glutAddSubMenu("Grid", menu_grid);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
		Keyboard(Empty, 0, 0);
	}
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(settings::width, settings::height); 
	glutCreateWindow("Текущий цвет всех точек:");
	Menu(Empty);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop();
}