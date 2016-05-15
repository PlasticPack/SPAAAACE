/*	Créateur: Marc-Antoine Céré
Projet: SPAAAACE
Fichier: AiComponent.cpp
But: Créer les composantes de l'intelligence artificielle
*/
#include "AiComponent.h"
using namespace std;

/*
	Destructeur de la classe AiComponent
*/
AiComponent::~AiComponent()
{
}

/*
	Fonction servant de constructeur initial et de constructeur de copie pour la génération
	de nouvelles A.I. lors de la génération de nouveaux niveaux.
	Réalisée par Charles Murphy.
*/
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

/*
	Opérateur '=' qui permet une plus grande facilité lors de la construction de copies.
	Réalisée par Charles Murphy.
*/
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

/*
	Fonction retournant le type de l'A.I. Ce type sera utile lors de la sélection de la conscience de l'A.I.
*/
int AiComponent::getType()
{
	return m_typeAi;
}

/*
	Retourne le vecteur direction de l'A.I.
*/
Vec2 AiComponent::getVec()
{
	return m_vec;
}

/*
	Permet d'initialisé le type de l'A.I.
*/
void AiComponent::setType(int type)
{
	m_typeAi = type;
}

void AiComponent::setVec(Vec2 vec)
{
	m_vec = vec;
}

/*
	Initialise les composantes physique de l'A.I.
*/
void AiComponent::setPhysicsComp(std::shared_ptr<PhysicsComponent> comp){
	this->m_phyComponent = comp;
}

/*
	Permet de retournet les composantes physiques de l'A.I.
*/
std::shared_ptr<PhysicsComponent> AiComponent::getPhysicsComponent(){
	return m_phyComponent;
}

/*
	Initialise le GameLogicComponent de l'A.I. qui contient la puissance des
	moteurs ainsi que la vie.
*/
void AiComponent::setGLComponent(std::shared_ptr<GameLogicComponent> comp){
	this->m_gameLogicComponent = comp;
}

std::shared_ptr<GameLogicComponent> AiComponent::getGLComponent(){
	return m_gameLogicComponent;
}

/*
	Initialise les composantes physiques de la variable m_target, qui sont celles
	du joueur. Elles seront utilisées plus tard pour les calculs de direction.
*/
void AiComponent::setTarget(std::shared_ptr<PhysicsComponent> target){
	m_target = target;
}

/*
	Retourne les composantes prisent du joueur.
*/
std::shared_ptr<PhysicsComponent> AiComponent::getTarget(){
	return m_target;
}

/*
	Initialise les composantes physique de la variable physique m_nearDanger,
	qui est l'objet le plus près de l'A.I. et qui pourrait être à éviter.
*/
void AiComponent::setNearDanger(std::shared_ptr <PhysicsComponent> nD){

	this->m_nearDanger = nD;
}

/*
	Retourne les propriétés physiques de la variable m_nearDanger.
*/
std::shared_ptr <PhysicsComponent> AiComponent::getNearDanger(){
	return m_nearDanger;
}

/*
	Fonction qui permet le lien entre l'A.I. et le scipt du jeu.
*/
AiComponent::AiComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PhysicsComponent> comp){
	using namespace luabridge;
	m_phyComponent = comp;
}

/*
	Initialise la puissance des moteurs de l'A.I. pwr provient de la GameLogicComponent.
*/
void AiComponent::setPower(int pwr){
	m_pwr = pwr;
}

/*
	Retourne la puissance des moteurs.
*/
int AiComponent::getPower(){
	return m_pwr;
}

/*
	Retourne true si l'A.I. est à moins de 500 pixels du m_nearDanger. Cette valeur booléenne permettra
	à l'AiSystem de savoir si l'A.I. doit poursuivre le joueur ou évité l'objet ou  la planète.
*/
bool AiComponent::isCloseToDanger(){
	if (this->getPhysicsComponent()->getPosition().getDist(this->getNearDanger()->getPosition()) <= (this->getNearDanger()->getHitboxRadius() + 500))
	{
		return true;
	}
	else{
		return false;
	}
}
