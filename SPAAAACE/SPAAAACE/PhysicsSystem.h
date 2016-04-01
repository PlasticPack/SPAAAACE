#pragma once

#include "PhysicsComponent.h"

class PhysicsComponent;

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

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	Vec2 gravity(PhysicsComponent &a, PhysicsComponent& b); // retourne la gravit� que b exerce sur this

	void update(Message &postman, PhysicsComponent &a, std::vector<std::shared_ptr<PhysicsComponent>> &phyComps, double dt);

	bool checkIfCollide(Message &postman, PhysicsComponent &a, PhysicsComponent& b, double dt);

	void resolveCollision(Message &postman,  std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt);


private :
	Vec2 accelerate(PhysicsComponent &initial, PhysicsComponent &bState, std::vector<std::shared_ptr<PhysicsComponent>>&, double dt);
	Derivative evaluate(PhysicsComponent &a, std::vector<std::shared_ptr<PhysicsComponent>> &phyComps, double dt, Derivative &d);
};

