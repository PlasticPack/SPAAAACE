#pragma once

#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LuaInit.hpp"

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

class Scene
{
public:

	Scene();
	~Scene();

	//void addGameObject();
	
	void update(Message &postman);
	void init(); // méthode qui prend un script  et initialise le vector de gameObjects

	//void addSkyBody(double x, double y, double mass, std::string id, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
	void orderByZIndex();

	//getters des components, utile à luafunctions.cpp
	template <typename T> void addComponent(std::shared_ptr<Component> c){
		 
		//m_components[t].push_back(c);
		std::cout << "Added a " << std::type_index(typeid(T)).name() << " to Scene\n";
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
	std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	std::vector<std::shared_ptr<PositionComponent>> m_posComps;
	std::vector<std::shared_ptr<GraphicsComponent>> m_graphicsComps;
	std::vector<std::shared_ptr<PhysicsComponent>> m_physicsComps;


	std::map<std::type_index, std::vector<std::shared_ptr<Component>>> m_components;

	InputsSystem m_inSystem;
	PhysicsSystem m_phySystem;
};

