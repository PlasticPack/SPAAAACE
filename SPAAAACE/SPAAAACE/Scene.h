#pragma once

#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "ActionSystem.h"
#include "GameLogicSystem.h"
#include "GameObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LuaInit.hpp"
#include <exception>

#include "XML_utilities.hpp"

/*********************************************

COMMENT CA MARCHE, 18 mars 2016

Pour ajouter un objet, vous éditez le script "nem1.lua"
(ou vous en créez un nouveau et le chargez). Ex :

MonObjet = {
	Component1 = {
		propriété1 = x,
		propriété2 = true,
		...
	},
	Component2 = {
		propriété1 = x,
		propriété2 = true,
		...
	},
	...
}
MonObjet2 = {
...
}

Notez bien les emplacement des virgules entre 
chaque propriété et chaque Component mais
pas entre les objets.
N'oubliez pas que pour que les Component
Graphics et Physics fonctionnent, vous devez
inclure un PositionComponent.
Une fois le script terminé, dirigez vous dans 
la méthode init() de la classe Scene, et 
copiez-collez cette ligne :
m_gameObjects.push_back(luain::loadGameObjects(this, L, "player"));
et remplacez "player" par, par exemple, "MonObjet"
Et voilà!
----------------------------------


**************************************************/

#include "Message.h"

class PhysicsSystem;

class Scene
{
public:

	Scene(std::string arg, std::string);
	~Scene();
	
	void update(Message &postman);
	void init(std::string arg); // méthode qui prend un script  et initialise le vector de gameObjects

	void orderByZIndex();

	//getters des components, utile à luafunctions.cpp
	template <typename T> void addComponent(std::shared_ptr<Component> c){
		 
		//m_components[t].push_back(c);
		//std::cout << "Added a " << std::type_index(typeid(T)).name() << " to Scene\n";
		if (std::type_index(typeid(T)) == std::type_index(typeid(PositionComponent))){
			m_posComps.push_back(std::dynamic_pointer_cast<PositionComponent>(c));
		}

		if (std::type_index(typeid(T)) == std::type_index(typeid(PhysicsComponent))){
			m_physicsComps.push_back(std::dynamic_pointer_cast<PhysicsComponent>(c));
		}

		if (std::type_index(typeid(T)) == std::type_index(typeid(GraphicsComponent))){
			m_graphicsComps.push_back(std::dynamic_pointer_cast<GraphicsComponent>(c));
		}
	}


	template <typename C> std::string getFatherID(std::shared_ptr<C> c){

		int j = 0;
		for (int i = 0; i < m_gameObjects.size(); i++){

			if (std::to_string((int)c.get()) == std::to_string((int)m_gameObjects[i]->get<C>().get()) ) {
				j = i;
				i = m_gameObjects.size();
			}
		}

		return m_gameObjects[j]->getID();
	}


	template <typename C> std::string getFatherID(int c){

		int j = 0;
		for (int i = 0; i < m_gameObjects.size(); i++){

			if (std::to_string(c) == std::to_string((int)m_gameObjects[i]->get<C>().get())) {
				j = i;
				i = m_gameObjects.size();
			}
		}

		return m_gameObjects[j]->getID();
	}

protected:
	std::vector<std::shared_ptr<GameObject>> m_gameObjects;

	std::vector<std::shared_ptr<PositionComponent>>		m_posComps;
	std::vector<std::shared_ptr<GraphicsComponent>>		m_graphicsComps;
	std::vector<std::shared_ptr<PhysicsComponent>>		m_physicsComps;
	std::vector<std::shared_ptr<GameLogicComponent>>	m_GLComps;
	std::vector<std::shared_ptr<ActionComponent>>		m_ActionComps;




	//std::vector<std::shared_ptr<AiComponent>> m_aiComps;
	//std::map<std::type_index, std::vector<std::shared_ptr<Component>>> m_components;

	std::string m_id;
	std::string m_focusedID;

	InputsSystem m_inSystem;

	LTimer m_navigationTimer;
	LTimer m_dialogueTimer;
};

