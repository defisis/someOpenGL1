#include "poligon.h"
#define _USE_MATH_DEFINES
#include <math.h>


void Poligon::recount()
{
	point t(_vertices[0].pos[0] - _center.pos[0], _vertices[0].pos[1] - _center.pos[1]);
	for (int i = 1; i < _vertices.size(); i++) {
		t = rotateVector(t, 2 * M_PI / constants::poligonSize);
		_vertices[i] = point(t.pos[0] + _center.pos[0], t.pos[1] + _center.pos[1]);
	}
}

point Poligon::rotateVector(point v, float angle)
{
	return point(v.pos[0] * cos(angle) - v.pos[1] * sin(angle), v.pos[1] * cos(angle) + v.pos[0] * sin(angle));
}

Poligon::Poligon(point center, point firstVertex) : _center{center}
{
	_vertices.resize(constants::poligonSize);
	_vertices[0] = firstVertex;
	_move.pos[0] = 0;
	_move.pos[1] = 0;
}

void Poligon::setTextureID(int textureID)
{
	_textureID = textureID;
}

void Poligon::setColor(GLubyte R, GLubyte G, GLubyte B)
{
	_center.color[0] = R;
	_center.color[1] = G;
	_center.color[2] = B;
}

void Poligon::setFirstVertex(point vertex)
{
	_vertices[0] = vertex;
	recount();
}

point Poligon::getCenter()
{
	return _center;
}

std::vector<point> Poligon::getVertices()
{
	return _vertices;
}

int Poligon::getTextureID()
{
	return _textureID;
}

float Poligon::getAngle()
{
	return _angle;
}

float Poligon::getScale()
{
	return _scale;
}

point Poligon::getMove()
{
	return _move;
}

void Poligon::rotate(float angle)
{
	_angle += angle;
}

void Poligon::scale(float scaleCoefficient)
{
	_scale += scaleCoefficient;
}

void Poligon::move(float x, float y)
{
	_move.pos[0] += x;
	_move.pos[1] += y;
}
