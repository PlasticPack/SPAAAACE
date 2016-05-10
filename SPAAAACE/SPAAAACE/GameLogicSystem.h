/*************************
Créé par Charles Murphy
Système gérant les GameLogicComponent
**************************/
#pragma once

#include "GameLogicComponent.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

class GameLogicSystem
{
public:
	static void init();
	static void close();

	static void update(Message &postman, std::shared_ptr<GameObject>, GameLogicComponent &comp, double dt);

protected:
	GameLogicSystem();
	~GameLogicSystem();
};

