#pragma once

#include "GameLogicComponent.h"
#include "GameObject.h"
#include "PhysicsComponent.h"

class GameLogicSystem
{
public:
	static void init();
	static void close();

	static void update(Message &postman, std::shared_ptr<GameObject>, GameLogicComponent &comp);

protected:
	GameLogicSystem();
	~GameLogicSystem();
};

