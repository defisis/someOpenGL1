#pragma once
#include "point.h"
#include <vector>
#include <array>
#include "constants.h"


class Poligon {
private:
	point _center;
	std::vector<point> _vertices;
	int _textureID = -1;
	GLubyte color[3];

	float _scale = 1;
	float _angle = 0;
	point _move;

	void recount();
	point rotateVector(point v, float angle);
public:
//	Poligon();
//	Poligon(point center);
	Poligon(point center, point firstVertex);

	void setTextureID(int textureID);
	void setColor(GLubyte R, GLubyte G, GLubyte B);
	void setFirstVertex(point vertex);

	point getCenter();
	std::vector<point> getVertices();
	int getTextureID();
	float getAngle();
	float getScale();
	point getMove();


	void rotate(float angle);
	void scale(float scaleCoefficient);
	void move(float x, float y);
};