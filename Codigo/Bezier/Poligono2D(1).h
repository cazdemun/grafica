#pragma once
#include "Punto2D.h"
#include <vector>

using namespace std;

 class Poligono2D {
 public:
	 vector <Punto2D> vertices;
	 int nV;

	 Poligono2D() {}
		 
	 Poligono2D(int nV)
	{
		this -> nV = nV;
		this->vertices = vector <Punto2D>(nV);
		for (int i = 0; i < nV; i++)
			this->vertices[i] = Punto2D();
	 }
	Punto2D getVertice(int p)
	{
		return vertices[p];
	}
	 void setVertice(int p, Punto2D v)
	{
		vertices[p].setX(v.getX());
		vertices[p].setY(v.getY());
	}
	 void setVertice(int p, double x, double y)
	{
		vertices[p].setX(x);
		vertices[p].setY(y);
	}
	 int cantidadVertices()
	{
		return nV;
	}

	 void Trasladar(double dx, double dy)
	{
		for (int i = 0; i < nV; i++)
			vertices[i].Trasladar(dx, dy);
	}
	 void Escalar(Punto2D base, double sx, double sy)
	{
		for (int i = 0; i < nV; i++)
			vertices[i].Escalar(base, sx, sy);
	}

	 void Rotar(Punto2D base, double a)
	{
		for (int i = 0; i < nV; i++)
			vertices[i].Rotar(base, a);
	}

 };

