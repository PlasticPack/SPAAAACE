/*************************
Créé par Charles Murphy
Système gérant les PhysicsComponent
et leurs interactions
CODE LIBREMENT INSPIRÉ DE
http://gafferongames.com/game-physics/integration-basics/
**************************/
#pragma once

#include "PhysicsComponent.h"
#include "Scene.h"

class PhysicsComponent;
struct State { // stocke l'état d'un objet (position/vélocité)
	Vec2 pos;
	Vec2 vel;
};

struct Derivative { // stocke la dérivée de l'état d'un objet (en fonction du temps)
	Vec2 dpos; // (dpos/dt = vélocité)
	Vec2 dvel; // (dvel/dt = accélération)
};

class Scene;

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	static Vec2 gravity(PhysicsComponent &a, PhysicsComponent& b); // retourne la gravité que b exerce sur a

	static void update(Message &postman, Scene *s, PhysicsComponent &a, std::vector<std::shared_ptr<PhysicsComponent>> &phyComps, double dt);

	static bool checkIfCollide(Message &postman, Scene* s, std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt);
	static bool checkIfCollide(PhysicsComponent &a, PhysicsComponent &b, double dt);
	static void resolveCollision(Message &postman, Scene *s, std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt);

	static void setGravity(double g){ m_gravity = g; }

private :
	static Vec2 accelerate(PhysicsComponent &initial, PhysicsComponent &bState, std::vector<std::shared_ptr<PhysicsComponent>>&, double dt);
	static Derivative evaluate(PhysicsComponent &a, std::vector<std::shared_ptr<PhysicsComponent>> &phyComps, double dt, Derivative &d);
	static double m_gravity;
};

