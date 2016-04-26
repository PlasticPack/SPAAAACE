#include "ActionComponent.h"
#include <LuaBridge.h>


ActionComponent::ActionComponent(luabridge::LuaRef& componentTable)
{
	m_cooldown = -1;
	m_on = true;
	m_timer.start();
	using namespace luabridge;

	auto trigger = componentTable["trigger"];
	auto answer = componentTable["answer"];
	auto timer = componentTable["timer"];

	if (trigger.isString() && answer.isString()){

		std::string trig = trigger.cast<std::string>();
		std::string answ = answer.cast<std::string>();

		//on sépare le string entre les " "
		//POUR TRIGGER
		m_trigger = stringToMessage(trig);

		//POUR ANSWER
		m_answer = stringToMessage(answ);
	}

	if (timer.isNumber()){
		m_cooldown = timer;
	}
}

typeSingleMessage stringToMessage(std::string  trig){
	typeSingleMessage  m;
	
	std::istringstream t_buf(trig);
	std::istream_iterator<std::string> t_beg(t_buf), end;
	std::vector<std::string> trig_strings(t_beg, end);

	for (int i = 0; i < trig_strings.size(); i++){
		std::replace(trig_strings[i].begin(), trig_strings[i].end(), '\\', ' ');
	}

	m.first.first = trig_strings[0];
	m.first.second = trig_strings[1];
	m.second.first = std::stoi(trig_strings[2]);
	m.second.second = std::stod(trig_strings[3]);

	return m;
}

ActionComponent::~ActionComponent()
{
}

