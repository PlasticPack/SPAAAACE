#pragma once

#include "GameLogicComponent.h"

class GameLogicSystem
{
public:
	static void init();
	static void close();

	static void update(Message &postman, std::string id, GameLogicComponent &comp);

protected:
	GameLogicSystem();
	~GameLogicSystem();
};

