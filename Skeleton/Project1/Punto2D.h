#pragma once
#include <cmath>

 class Punto2D {
 private: 
	 double x, y;

 public:
	 Punto2D()
	{
		x = y = 0;
	}
	 Punto2D(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	 double getX() {
		return x;
	}
	 void setX(double x) {
		this->x = x;
	}
	 double getY() {
		return y;
	}
	 void setY(double y) {
		this->y = y;
	}

	 void Trasladar(double dx, double dy)
	{
		x = dx + x;  // x += dx;
		y = dy + y;  // y += dy;
	
	}
	 void Escalar(double sx, double sy)
	{
		x = sx * x;
		y = sy * y;
		
	}
	 void Escalar(Punto2D *base, double sx, double sy)
	{
		Trasladar(-base->getX(), -base->getY());
		Escalar(sx, sy);
		Trasladar(base->getX(), base->getY());
		

	}
	 void Rotar(double a)
	{
		double ca, sa, xp;

		ca = cos(a);
		sa = sin(a);
		xp = x * ca - y * sa;
		y = x * sa + y * ca;
		x = xp;
	}

	 void Rotar(Punto2D* base, double a)
	{
		Trasladar(-base->getX(), -base->getY());
		Rotar(a);
		Trasladar(base->getX(), base->getY());
		
	}






 };
