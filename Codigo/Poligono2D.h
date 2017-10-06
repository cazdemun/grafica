#pragma once
#include "Punto2D.h"
#include <vector>

using namespace std;

 class Poligono2D {
 public:
	 vector <Punto2D> vertices;
	 int nV;

	 Poligono2D(){}

	 Poligono2D(int nV)
	{
		this-> nV = nV;
		vertices = vector <Punto2D>(nV);
		for (int i = 0; i < nV; i++) {
			
			this->vertices.at(i) = Punto2D();
		}
	}
	Punto2D getVertice(int p)
	{
		return vertices.at(p);
	}

	void trasladarVertice(double dx, double dy, int j) {
		vertices[j].Trasladar(dx, dy);
	}

	 void setVertice(int p, Punto2D v)
	{
		vertices.at(p).setX(v.getX());
		vertices.at(p).setY(v.getY());
	}
	 void setVertice(int p, double x, double y)
	{
		vertices.at(p).setX(x);
		vertices.at(p).setY(y);
	}
	 int CantidadVertices()
	{
		return nV;
	}

	 void Trasladar(double dx, double dy)
	{
		for (int i = 0; i < nV; i++)
			vertices.at(i).Trasladar(dx, dy);
	}
	 void Escalar(Punto2D base, double sx, double sy)
	{
		for (int i = 0; i < nV; i++)
			vertices.at(i).Escalar(base, sx, sy);
	}

	 void Rotar(Punto2D base, double a)
	{
		for (int i = 0; i < nV; i++)
			vertices.at(i).Rotar(base, a);
	}

 };

