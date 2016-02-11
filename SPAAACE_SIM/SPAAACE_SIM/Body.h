#pragma once

#include "Vec2.h"

/*******************************
Classe qui représente un objet 2D
avec une position, une masse, une
vélocité, et l'ensemble des forces
qui agissent sur lui
*********************************/

class Body
{
public:
	Body();
	Body(Vec2 pos, Vec2 vel, double mass, double breakP, double radiusHB, Vec2 forces);
	Body(const Body& b);
	~Body();
	
	void operator=(const Body& b);

	void applyForce(Vec2 force);

	Vec2 getPosition();
	double getMass();
	Vec2 getVelocity();
	Vec2 getForces();
	double getBreakPoint();
	double getHitboxRadius();
	bool isSolid();
	bool isFixed();
	double getElasticity();

	void setMass(double mass);
	void setForces(Vec2 f);
	void resetForces();

	void setVelocity(Vec2 vel);
	void setPosition(Vec2 pos);
	void setHitboxRadius(double n);
	void setSolid(bool s);
	void setFixed(bool f);
	void setElasticity(double e);
	
protected:
	Vec2 m_position;
	double m_mass;
	Vec2 m_velocity;
	Vec2 m_forces;
	double m_breakPoint;
	double m_elasticity;
	double m_hitboxRadius;
	bool m_solid;
	bool m_fixed;
};

