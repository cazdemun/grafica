#pragma once
#include "Poligono2D.h"
#include <vector>

using namespace std;

Poligono2D pol;
Punto2D vpol;

class Malla2D {
public:
	vector<Poligono2D> poligonos;

	Malla2D() {
		this->poligonos = vector <Poligono2D>();
	}

	 void addPoligono(Poligono2D pol)
	{
		poligonos.push_back(pol);
	}
	 int getCantidadPoligonos()
	{
		return poligonos.size();
	}
	 Poligono2D getPoligono(int p)
	{
		return poligonos[p];
	}

	 void Trasladar(Punto2D vertice, double dx, double dy)
	{
		int i, j;

		for (i = 0; i<getCantidadPoligonos();i++)
		{
			pol = getPoligono(i);
			for (j = 0; j < pol.CantidadVertices();j++)
			{
				vpol = poligonos[i].getVertice(j);

				if (vpol.equals(vertice)) 
					poligonos[i].trasladarVertice(dx, dy, j);
				
				}
		}
	}

};