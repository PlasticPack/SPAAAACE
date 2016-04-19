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
		for (auto& s = g.m_components.begin(); s != g.m_components.end(); s++){

			//std::cout << "\nover here!";
			//addComponent(it->first, std::make_shared<T>(*it->second));

			if (s->first == std::type_index(typeid(PositionComponent))){
				addComponent(s->first, std::make_shared<PositionComponent>(*std::dynamic_pointer_cast<PositionComponent>(s->second)));
				//m_posComps.push_back(std::dynamic_pointer_cast<PositionComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(PhysicsComponent))){
				addComponent(s->first, std::make_shared<PhysicsComponent>(*std::dynamic_pointer_cast<PhysicsComponent>(s->second)));
				//m_physicsComps.push_back(std::dynamic_pointer_cast<PhysicsComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(GraphicsComponent))){
				addComponent(s->first, std::make_shared<GraphicsComponent>(*std::dynamic_pointer_cast<GraphicsComponent>(s->second)));
				//m_graphicsComps.push_back(std::dynamic_pointer_cast<GraphicsComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(GameLogicComponent))){
				addComponent(s->first, std::make_shared<GameLogicComponent>(*std::dynamic_pointer_cast<GameLogicComponent>(s->second)));
				//m_GLComps.push_back(std::dynamic_pointer_cast<GameLogicComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(ActionComponent))){
				addComponent(s->first, std::make_shared<ActionComponent>(*std::dynamic_pointer_cast<ActionComponent>(s->second)));
				//m_ActionComps.push_back(std::dynamic_pointer_cast<ActionComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(AiComponent))){
				addComponent(s->first, std::make_shared<AiComponent>(*std::dynamic_pointer_cast<AiComponent>(s->second)));
				//m_AiComps.push_back(std::dynamic_pointer_cast<AiComponent>(s.second));
			}

		}
	}
}

GameObject& GameObject::operator=(GameObject const& g){
	if (this != &g){
		m_ID = g.m_ID;
		m_components.clear();
		for (auto& s = g.m_components.begin(); s != g.m_components.end(); s++){

			//addComponent(it->first, std::make_shared<T>(*it->second));

		/*	if (s->first == std::type_index(typeid(PositionComponent))){
				addComponent(s->first, std::make_shared<PositionComponent>(*s->second));
				//m_posComps.push_back(std::dynamic_pointer_cast<PositionComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(PhysicsComponent))){
				addComponent(s->first, std::make_shared<PhysicsComponent>(*s->second));
				//m_physicsComps.push_back(std::dynamic_pointer_cast<PhysicsComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(GraphicsComponent))){
				addComponent(s->first, std::make_shared<GraphicsComponent>(*s->second));
				//m_graphicsComps.push_back(std::dynamic_pointer_cast<GraphicsComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(GameLogicComponent))){
				addComponent(s->first, std::make_shared<GameLogicComponent>(*s->second));
				//m_GLComps.push_back(std::dynamic_pointer_cast<GameLogicComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(ActionComponent))){
				addComponent(s->first, std::make_shared<ActionComponent>(*s->second));
				//m_ActionComps.push_back(std::dynamic_pointer_cast<ActionComponent>(s.second));
			}

			if (s->first == std::type_index(typeid(AiComponent))){
				addComponent(s->first, std::make_shared<AiComponent>(*s->second));
				//m_AiComps.push_back(std::dynamic_pointer_cast<AiComponent>(s.second));
			}*/

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