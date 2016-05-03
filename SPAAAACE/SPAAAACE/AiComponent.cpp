/*	Créateur: Marc-Antoine Céré
Projet: SPAAAACE
Fichier: AiComponent.cpp
But: Créer les composantes de l'intelligence artificielle
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

		if (a.m_target != nullptr)
			m_target = a.m_target;
		
		if (a.m_nearDanger != nullptr)
			m_nearDanger = std::make_shared<PhysicsComponent>(*a.m_nearDanger);

		if (a.m_phyComponent != nullptr)
			m_phyComponent = std::make_shared<PhysicsComponent>(*a.m_phyComponent);
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

		if (a.m_target != nullptr)
			m_target = a.m_target;

		if (a.m_nearDanger != nullptr)
			m_nearDanger = std::make_shared<PhysicsComponent>(*a.m_nearDanger);

		if (a.m_phyComponent != nullptr)
			m_phyComponent = std::make_shared<PhysicsComponent>(*a.m_phyComponent);
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

void AiComponent::setGLComponent(std::shared_ptr<GameLogicComponent> comp){
	this->m_gameLogicComponent = comp;
}

std::shared_ptr<GameLogicComponent> AiComponent::getGLComponent(){
	return m_gameLogicComponent;
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

void AiComponent::setPower(int pwr){
	m_pwr = pwr;
}

int AiComponent::getPower(){
	return m_pwr;
}


bool AiComponent::isCloseToDanger(){
	if (this->getPhysicsComponent()->getPosition().getDist(this->getNearDanger()->getPosition()) <= (this->getNearDanger()->getHitboxRadius() + 500))
	{
		return true;
	}
	else{
		return false;
	}
}
