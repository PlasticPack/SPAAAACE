/*************************
Créé par Charles Murphy
Système gérant les objectifs du joueur
**************************/
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

	void init();
	void addObjective(luabridge::LuaRef& componentTable);
	void update(Message &postman, std::map<std::string, std::shared_ptr<GameObject>> &objs);
	std::string getCurrentObjective() { return m_currentObjective; }
	std::string getCurrentTarget() { return m_objectives[m_currentObjective].targetID; }
	Vec2 getObjPosition() { return m_objPosition; };
	void setObjPosition(Vec2 v) { m_objPosition = v; };
	s_objective getObjective(std::string l) { return m_objectives[l]; }
	void activateObjective(std::string name, bool a = true) { if(m_objectives.find(name) != m_objectives.end())  m_objectives[name].done = a; }

protected:
	//liste des objectifs
	std::map<std::string, s_objective> m_objectives;
	std::string m_currentObjective;
	Vec2 m_objPosition;
};

Vec2 getPointerPosition(Vec2 dir, int half_size);
