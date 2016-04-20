/*	Créateur: Marc-Antoine Céré
Projet: test Ai
Fichier: Ai.cpp
But: Créer une ai qui détecte les objets dans sa vision
*/
#include "AiComponent.h"
using namespace std;


AiComponent::~AiComponent()
{
}

AiComponent::AiComponent(AiComponent const& a){
	if (this != &a){
		m_acc = a.m_acc;
		m_vec = a.m_vec;
		m_typeAi = a.m_typeAi;

		m_nearDanger.reset();
		m_phyComponent.reset();
		m_target.reset();

		m_target = a.m_target;
		m_nearDanger = std::make_shared<PhysicsComponent>(*a.m_nearDanger);
		m_target = std::make_shared<PhysicsComponent>(*a.m_target);
	}
}


AiComponent& AiComponent::operator=(AiComponent const& a){
	if (this != &a){
		m_acc = a.m_acc;
		m_vec = a.m_vec;
		m_typeAi = a.m_typeAi;

		m_nearDanger.reset();
		m_phyComponent.reset();
		m_target.reset();

		m_target = a.m_target;
		m_nearDanger = std::make_shared<PhysicsComponent>(*a.m_nearDanger);
		m_target = std::make_shared<PhysicsComponent>(*a.m_target);
	}
	return *this;
}

int AiComponent::getType()
{
	return m_typeAi;
}


Vec2 AiComponent::getVec()
{
	return m_vec;
}

int AiComponent::getAcc(){
	return m_acc;
}



void AiComponent::setType(int type)
{
	m_typeAi = type;
}

void AiComponent::setVec(Vec2 vec)
{
	m_vec = vec;
}

void AiComponent::setPhysicsComp(std::shared_ptr<PhysicsComponent> comp){
	this->m_phyComponent = comp;
}

std::shared_ptr<PhysicsComponent> AiComponent::getPhysicsComponent(){
	return m_phyComponent;
}

void AiComponent::setTarget(std::shared_ptr<PhysicsComponent> target){
	m_target = target;
}

std::shared_ptr<PhysicsComponent> AiComponent::getTarget(){
	return m_target;
}

void AiComponent::setNearDanger(std::shared_ptr <PhysicsComponent> nD){

	this->m_nearDanger = nD;
}

std::shared_ptr <PhysicsComponent> AiComponent::getNearDanger(){
	return m_nearDanger;
}

AiComponent::AiComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PhysicsComponent> comp){
	using namespace luabridge;
	m_phyComponent = comp;
}
