#pragma once

#include "GameObject.h"
#include "ActionSystem.h"
#include "GameLogicSystem.h"
#include "GraphicsSystem.h"

struct s_objective{

	std::string name;
	std::string targetID;
	bool done;
};

class MissionSystem
{
public:
	MissionSystem();
	~MissionSystem();

	static void init();
	static void addObjective(luabridge::LuaRef& componentTable);
	static void update(Message &postman, std::map<std::string, std::shared_ptr<GameObject>> &objs);
	static std::string getCurrentObjective() { return m_currentObjective; }
	static std::string getCurrentTarget() { return m_objectives[m_currentObjective].targetID; }
	static Vec2 getObjPosition() { return m_objPosition; };
	static void setObjPosition(Vec2 v) { m_objPosition = v; };


protected:
	//liste des objectifs
	static std::map<std::string, s_objective> m_objectives;
	static std::string m_currentObjective;
	static Vec2 m_objPosition;
};

Vec2 getPointerPosition(Vec2 dir, int half_size);