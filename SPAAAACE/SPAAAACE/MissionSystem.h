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
	static void addObjective(luabridge::LuaRef& componentTable);
	static void update(Message &postman);
	static std::string getCurrentObjective() { return m_currentObjective; }
	static Vec2 getObjPosition() { return m_objPosition; };
	static void setObjPosition(Vec2 v) { m_objPosition = v; };


protected:
	//liste des objectifs
	static std::map<std::string, std::pair<std::string, bool>> m_objectives;
	static std::string m_currentObjective;
	static Vec2 m_objPosition;
};

Vec2 getPointerPosition(Vec2 dir, int half_size);