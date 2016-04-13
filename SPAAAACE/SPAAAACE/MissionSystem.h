#pragma once

#include "GameObject.h"
#include "ActionSystem.h"
#include "GameLogicSystem.h"
#include "GraphicsSystem.h"


class MissionSystem
{
public:
	MissionSystem();
	~MissionSystem();

	static void init();
	static void update(Message &postman);

protected:
	//liste des objectifs
	static std::map<std::string, bool> m_objectives;
	static std::string m_currentObjective;
};

