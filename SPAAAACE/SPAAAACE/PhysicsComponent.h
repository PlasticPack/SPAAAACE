#pragma once

#include "PositionComponent.h"
#include <vector>
#include <memory>


class PhysicsComponent : public Component
{
public:
	PhysicsComponent(std::shared_ptr<PositionComponent> comp);
	PhysicsComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PositionComponent> comp=nullptr);
	~PhysicsComponent();

	Vec2 getPosition();
	Vec2 getVelocity();
	Vec2 getForces();
	std::shared_ptr<PositionComponent> getPositionComponent();
	double getMass();
	double getHitboxRadius();
	double getElasticity() { return m_elasticity; };

	void setPosition(Vec2 pos);
	void setVelocity(Vec2 vel);
	void setForces(Vec2 forces);
	void setMass(double mass);
	void setHitboxRadius(double hbr);
	void setPositionComp(std::shared_ptr<PositionComponent> comp);

	bool isActive() { return m_activated; }

	void activate(bool a){
		m_activated = a;
	}

	/*Vec2 gravity(PhysicsComponent& b); // retourne la gravité que b exerce sur this
	
	void update(std::vector<PhysicsComponent> &phyComps, double dt);

	bool checkIfCollide(PhysicsComponent& b, double dt);*/

protected:
	std::shared_ptr<PositionComponent> m_posComponent;

	double m_mass;
	Vec2 m_velocity;
	Vec2 m_forces;
	double m_hitboxRadius;
	bool m_activated;
	double m_elasticity;
	//double m_friction;
};

