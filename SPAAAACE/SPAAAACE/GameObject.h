#pragma once

#include "AiComponent.h"
#include "ActionComponent.h"
#include "GameLogicComponent.h"
#include "GraphicsComponent.h"
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <string>
class GameObject
{
public:
	GameObject();
	GameObject(std::string);
	GameObject(GameObject const&);
	~GameObject();

	GameObject& operator=(GameObject const&);

	template <typename T> std::shared_ptr<T> get(){
		
		auto it = m_components.find(std::type_index(typeid(T))); // on trouve la position du component

		if (it != m_components.end()) {
			//std::cout << "\nFOUND COMP : " << std::type_index(typeid(T)).name();

			return std::dynamic_pointer_cast<T>(it->second); // on retourne le component si trouv�
		}
		return nullptr;
	}

	unsigned int getNumberOfComponents() { return m_components.size(); }
	std::string getID(){ return m_ID; }

	void addComponent(std::type_index, std::shared_ptr<Component> c);
	void removeComponent(std::type_index);
	
	bool hasComponent(std::type_index);
	void setID(std::string id){ m_ID = id; }

	void idSet(bool b){ m_idSet = b; }
	bool idSet() { return m_idSet; }

	std::map<std::type_index, std::shared_ptr<Component>> getComponents() { return m_components; }

	//bool operator<(GameObject &comp);

protected:
	std::map<std::type_index, std::shared_ptr<Component>> m_components;
	std::string m_ID;
	bool m_idSet;
};

