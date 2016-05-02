#ifndef ACTIONCOMPONENT_H
#define ACTIONCOMPONENT_H
#include "Component.h"
#include <vector>
#include <sstream>
#include "LTimer.h"
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

	void setTrigger(typeSingleMessage t) { m_trigger = t; }
	void setAnswer(typeSingleMessage a) { m_answer = a; }

	int getCooldown() { return m_cooldown; }
	void resetTimer() { m_timer.stop(); m_timer.start(); }
	LTimer getTimer() { return m_timer; }
	bool isOn() { return m_on; }
	void set(bool onOrOff) { m_on = onOrOff; }

protected:
	bool m_on;
	typeSingleMessage m_trigger;
	typeSingleMessage m_answer;
	LTimer m_timer;
	int m_cooldown;
};

typeSingleMessage stringToMessage(std::string  trig);

#endif 