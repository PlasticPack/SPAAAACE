#ifndef MAIN_PHYSICS_H
#define MAIN_PHYSICS_H


#pragma once

#include "Body.h"
#include <iostream>
#include <vector>

/***********************************
Namespace qui contient l'ensemble
des fonctions relatives � la gestion
de la physique
***********************************/

namespace Physics {

	//CODE LIBREMENT INSPIR� DE
	//http://gafferongames.com/game-physics/integration-basics/
	struct State { // stocke l'�tat d'un objet (position/v�locit�)
		Vec2 pos;
		Vec2 vel;
	};

	struct Derivative { // stocke la d�riv�e de l'�tat d'un objet (en fonction du temps)
		Vec2 dpos; // (dpos/dt = v�locit�)
		Vec2 dvel; // (dvel/dt = acc�l�ration)
	};
	
	void collide(Body &a, Body &b, double dt); // modifie les vitesses de 2 objets en collision
	Vec2 gravity(Body &a, Body &b); // modifie les forces totales sur 2 corps SELON GRAVITY

	bool checkIfCollide(Body &a, Body &b, double dt);
	
	Derivative evaluate(Body &initial, std::vector<Body> &bodies, double t, double dt, Derivative &d); //�value la d�riv�e d'un State
	void move(Body &a, std::vector<Body> &bodies, double t, double dt);
	Vec2 accelerate(Body& initial, Body &b, std::vector<Body> &bodies, double dt);
	
	int max(int a, int b);
	int min(int a, int b);
}

#endif MAIN_PHYSICS_H