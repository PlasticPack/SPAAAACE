#pragma once
#include "Component.h"
#include <vector>
#include <sstream>
#include <iterator>

typedef std::pair<std::pair<std::string, std::string>, std::pair<int, double>> typeSingleMessage;

/***********************************

Check si recu un message, et si recu
en renvoit un précis

***********************************/

class ActionComponent :
	public Component
{
public:
	ActionComponent(luabridge::LuaRef& componentTable);
	virtual ~ActionComponent();

	typeSingleMessage getTrigger() { return m_trigger; }
	typeSingleMessage getAnswer() { return m_answer; }

protected:

	typeSingleMessage m_trigger;
	typeSingleMessage m_answer;

};

