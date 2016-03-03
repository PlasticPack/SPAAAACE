#pragma once

#include "GameObject.h"
#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include <vector>
#include "Message.h"

class Scene
{
public:
	Scene();
	~Scene();

	void addGameObject();
	
	void update(Message &postman);
	void init(); // méthode qui prend un script  et initialise le vector de gameObjects

	void addSkyBody(double x, double y, double mass);

	//std::vector<GameObject> getObjects() { return m_gameObjects; } // temporaire
	/*
	template <typename T> std::vector<T> getAll(){ // retourne tous les components d'un type chez les objets
		std::vector<T> components;
		for (int i = 0; i < m_gameObjects.size(); i++){
			auto comp = m_gameObjects[i].get<T>();
			if (comp != nullptr){
				components.push_back(*comp);
			}
		}
	}*/

protected:
	std::vector<GameObject> m_gameObjects;
	std::vector<std::shared_ptr<PositionComponent>> m_posComps;
	std::vector<std::shared_ptr<GraphicsComponent>> m_graphicsComps;
	std::vector<std::shared_ptr<PhysicsComponent>> m_physicsComps;
	std::vector<std::shared_ptr<InputsComponent>> m_inputsComps;

	InputsSystem m_inSystem;
	PhysicsSystem m_phySystem;
	GraphicsSystem m_graSystem;
};

