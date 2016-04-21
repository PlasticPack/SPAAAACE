#include "GameObject.h"


GameObject::GameObject(std::string id) : m_ID(id)
{
}

GameObject::GameObject() : m_ID("NO_ID")
{
}

GameObject::GameObject(GameObject const& g){
	if (this != &g){
		//on copie PAS L'ID EXACT
		//bon, pour le moment on fait ca, question de truc de vector
		m_ID = g.m_ID;
		m_components.clear();
		for (auto it = g.m_components.begin(); it != g.m_components.end(); it++){

			//std::cout << "\nover here!";
			addComponent(it->first, it->second);

		}
	}
}

GameObject& GameObject::operator=(GameObject const& g){
	if (this != &g){
		m_components.clear();
		for (auto it = g.m_components.begin(); it != g.m_components.end(); it++){
			
			//std::shared_ptr<Component> newComp = std::make_shared<Component>(it->second);
			addComponent(it->first, it->second);
		}
	}
	return *this;
}
/*
bool GameObject::operator<(GameObject &g)
{
	if (this != &g){
		
		if (hasComponent(std::type_index(typeid(PositionComponent))) && g.hasComponent(std::type_index(typeid(PositionComponent)))) {
			return (&get<PositionComponent>() > &g.get<PositionComponent>());
		}

	}
	return false;
}*/

GameObject::~GameObject()
{
}

void GameObject::addComponent(std::type_index t, std::shared_ptr<Component> c) {
	
	m_components[t] = c;
}

void GameObject::removeComponent(std::type_index t) {
	auto it = m_components.find(t);
	if (it != m_components.end()) {
		m_components.erase(it);
	}
}


bool GameObject::hasComponent(std::type_index t){

	auto it = m_components.find(t); // on trouve la position du component
	if (it != m_components.end()) {
		return true;
	}

	return false;
}