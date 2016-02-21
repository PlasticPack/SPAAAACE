#include "Body.h"


Body::Body() : m_mass(1), m_solid(true), m_fixed(false), m_breakPoint(1), m_hitboxRadius(10), m_elasticity(1)
{
}

Body::Body(Vec2 pos, Vec2 vel, double mass, double breakP, double radiusHB, Vec2 forces) : m_position(pos), m_velocity(vel), m_mass(mass), m_breakPoint(breakP), m_hitboxRadius(radiusHB), m_forces(forces) {
	m_solid = true;
	m_fixed = false;
	m_elasticity = 1;
}

Body::Body(const Body& b){
	if (this != &b){
		m_position = b.m_position;
		m_velocity = b.m_velocity;
		m_mass = b.m_mass;
		m_forces = b.m_forces;
		m_hitboxRadius = b.m_hitboxRadius;
		m_solid = b.m_solid;
		m_fixed = b.m_fixed;
		m_elasticity = b.m_elasticity;
	}
}

void Body::operator=(const Body& b){
	if (this != &b){
		m_position = b.m_position;
		m_velocity = b.m_velocity;
		m_mass = b.m_mass;
		m_forces = b.m_forces;
		m_hitboxRadius = b.m_hitboxRadius;
		m_solid = b.m_solid;
		m_fixed = b.m_fixed;
		m_elasticity = b.m_elasticity;
	}
}

void Body::setSolid(bool s){
	m_solid = s;
}

void Body::setElasticity(double d){
	m_elasticity = d;
}

double Body::getElasticity(){
	return m_elasticity;
}

bool Body::isSolid(){
	return m_solid;
}

void Body::setFixed(bool s){
	m_fixed = s;
}

bool Body::isFixed(){
	return m_fixed;
}

Body::~Body()
{
}

Vec2 Body::getPosition() {
	return m_position;
}

double Body::getMass() {
	return m_mass;
}

void Body::setMass(double mass){
	m_mass = mass;
}

Vec2 Body::getVelocity(){
	return m_velocity;
}

Vec2 Body::getForces() {
	return m_forces;
}

void Body::resetForces(){
	m_forces *= 0;
}

void Body::setForces(Vec2 f){
	m_forces = f;
}

void Body::applyForce(Vec2 f){
	m_forces += f;
}

void Body::setVelocity(Vec2 vel){
	m_velocity = vel;
}

void Body::setPosition(Vec2 pos){
	if (!m_fixed)
		m_position = pos;
}

double Body::getBreakPoint(){
	return m_breakPoint;
}

double Body::getHitboxRadius() {
	return m_hitboxRadius;
}

void Body::setHitboxRadius(double n) {
	m_hitboxRadius = n;
}