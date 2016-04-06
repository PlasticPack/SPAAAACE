#pragma once

#include "PhysicsComponent.h"
#include "Scene.h"

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

class Scene;

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	static Vec2 gravity(PhysicsComponent &a, PhysicsComponent& b); // retourne la gravit� que b exerce sur this

	static void update(Message &postman, Scene *s, PhysicsComponent &a, std::vector<std::shared_ptr<PhysicsComponent>> &phyComps, double dt);

	static bool checkIfCollide(Message &postman, Scene* s, std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt);

	static void resolveCollision(Message &postman, Scene *s, std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt);


private :
	static Vec2 accelerate(PhysicsComponent &initial, PhysicsComponent &bState, std::vector<std::shared_ptr<PhysicsComponent>>&, double dt);
	static Derivative evaluate(PhysicsComponent &a, std::vector<std::shared_ptr<PhysicsComponent>> &phyComps, double dt, Derivative &d);
};

