#include "PhysicsComponent.h"
#include <LuaBridge.h>

//Les blagues ont une grasieuster de Justin :)


PhysicsComponent::PhysicsComponent(std::shared_ptr<PositionComponent> c)
{
	m_activated = true;
	m_posComponent = c;
	m_mass = 1.0;
	//Si j'ai trois jeunes dans une autobus qui va vers l'école combien il a de jambre dans cette autobus ?
	m_hitboxRadius = 15;
	m_elasticity = 0.9;
	setHitboxRadius(50.0);
	m_angularVelocity = 0;
}

#include <string>

PhysicsComponent::PhysicsComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PositionComponent> c){
	using namespace luabridge;
	m_posComponent = c;
	m_elasticity = 0.9;
	m_angularVelocity = 0;
	m_activated = true;
	m_velocity = Vec2(0, 0);

	auto massRef =		componentTable["mass"];
	auto activateRef =	componentTable["activated"];
	auto hbRef =		componentTable["hitbox"];
	auto ela =			componentTable["elasticity"];
	auto xVel =			componentTable["vel_x"];
	auto yVel =			componentTable["vel_y"];
	auto aVel =			componentTable["vel_a"];
	
	if (massRef.isNumber()){
		setMass(massRef.cast<double>());
		//std::cout << "Mass is : " << massRef.cast<double>();
	}
	else {
		setMass(1.0);
		std::cout << "Mass is : " << getMass();
	}

	if (activateRef.isString()){
		if (strcmp(activateRef.cast<std::string>().c_str(), std::string("False").c_str()) == 0 || 
			strcmp(activateRef.cast<std::string>().c_str(), std::string("false").c_str()) == 0){
			m_activated = 0;
		}
		else m_activated = 1;
	}

	if (xVel.isNumber() && yVel.isNumber()){
		m_velocity = Vec2(xVel, yVel);
	}
	
	if (aVel.isNumber()){
		m_angularVelocity = aVel;
	}

	if (hbRef.isNumber()){
		setHitboxRadius(hbRef.cast<double>());
	}
	else setHitboxRadius(15);

	if (ela.isNumber()){
		m_elasticity = ela.cast<double>();
	}

	if (hbRef.isNumber()){
		setHitboxRadius(hbRef.cast<double>());
	}
	else setHitboxRadius(50);
	
}


PhysicsComponent::~PhysicsComponent()
{
}


Vec2 PhysicsComponent::getVelocity(){
	return m_velocity;
}

Vec2 PhysicsComponent::getPosition(){
	return m_posComponent->getPosition();
}

Vec2 PhysicsComponent::getForces(){
	return m_forces;
}

double PhysicsComponent::getMass(){
	return m_mass;
	//Gab fait de la musique si sort des auparleurs
}

double PhysicsComponent::getHitboxRadius(){
	return m_hitboxRadius;
}

std::shared_ptr<PositionComponent> PhysicsComponent::getPositionComponent(){
	return m_posComponent;
}

void PhysicsComponent::setPosition(Vec2 pos){
	if (!m_posComponent->isFixed())
		m_posComponent->setPosition(pos);
}

void PhysicsComponent::setVelocity(Vec2 pos){
	m_velocity = pos;
}

void PhysicsComponent::setForces(Vec2 pos){
	m_forces = pos;
	//LUA LUA LUA LUA LUA LUALUALUA  LUA LUA LUALUA LUA LUALUA  v  vLUA LUALUALUA LUA LUA LUALUA LUA LUA LUALUA v LUA
}

void PhysicsComponent::setMass(double pos){
	m_mass = pos;
}

void PhysicsComponent::setHitboxRadius(double pos){
	m_hitboxRadius = pos;
}

void PhysicsComponent::setPositionComp(std::shared_ptr<PositionComponent> comp){
	this->m_posComponent = comp;
}