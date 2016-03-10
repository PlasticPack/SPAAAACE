#pragma once

#include "GameObject.h"
#include "GraphicsSystem.h"
#include "PhysicsSystem.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*********************************************

COMMENT CA MARCHE, EN GROS :
TOUT se passe à partir d'une instance de la classe Scene.
En principe, on passe un script à la scène en question à 
sa méthode init(). Ce script contient toutes les propriétés 
de la scène : les objets et leurs positions, les background,
etc. Voir plus bas pour savoir comment écrire ce script plus
en détails.
Pour le moment, le script est hard-coded dans la seule instance
de scène, car je n'ai pas encore accès aux méthodes de Tristan.

-----------------------------------

Comment écrire ledit script :
Le script est composé de deux choses :
A - un ensemble de fonctions créant un type d'objet particulier, ex:
createAsteroid(Vec2 position) {
	Sprite asteroidSpr("asteroid.png", ...);
	m_posComps.push_back(std::make_shared<PositionComponent>())															//ajoute un positionComponent aux components de la Scene
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps.back()));									//ajoute un physicsComponent   ''  ''	...	  
	m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>())));		//ajoute un graphicsComponent  ''  ''	...   
	
	m_gameObjects.push_back(GameObject("asteroid_x"));														//ajoute l'objet en tant que tel
	m_gameObjects.back().addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps.back());	//greffe les components précédement ajoutés à l'objet
	m_gameObjects.back().addComponent(std::type_index(typeid(GraphicsComponent)), m_graphicsComps.back());	// ''

	m_gameObjects.back().get<PhysicsComponent>()->setPosition(position);	//set les paramètres précis des composantes (ici physique+position)
	m_gameObjects.back().get<PhysicsComponent>()->setMass(mass);
}
ou encore
createButton(){
	//meme machins, mais avec les composantes dont un bouton a besoin
}
Donc, en gros : AJOUTER COMPONENTS À LA SCENE -> CRÉER UN GAMEOBJECT VIDE -> Y AJOUTER LES COMPONENTS -> AJUSTER SES PARAMÈTRES 
B - un ensemble d'instructions utilisant les fonctions précédement créées, ex:
for(int i = 0; i < 5; i++){
	createAsteroid({position au hasard});
}
ou
//création d'un menu
createButton("PLAY", ...);
createButton("QUIT", ...);
createButton("OPTIONS", ...);

----------------------------------



**************************************************/


class Scene
{
public:
	Scene();
	~Scene();

	void addGameObject();
	
	void update();
	void init(); // méthode qui prend un script  et initialise le vector de gameObjects

	void addSkyBody(double x, double y, double mass, std::string id, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
	void orderByZIndex();

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

	PhysicsSystem m_phySystem;
	GraphicsSystem m_graSystem;
};

