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
		std::istringstream t_buf(trig);
		std::istream_iterator<std::string> t_beg(t_buf), end;
		std::vector<std::string> trig_strings(t_beg, end);

		for (int i = 0; i < trig_strings.size(); i++){
			std::replace(trig_strings[i].begin(), trig_strings[i].end(), '\\', ' ');
		}

		m_trigger.first.first = trig_strings[0];
		m_trigger.first.second = trig_strings[1];
		m_trigger.second.first =  std::stoi(trig_strings[2]);
		m_trigger.second.second = std::stod(trig_strings[3]);

		//POUR ANSWER
		std::istringstream a_buf(answ);
		std::istream_iterator<std::string> a_beg(a_buf), aend;
		std::vector<std::string> answ_strings(a_beg, aend);

		for (int i = 0; i < answ_strings.size(); i++){
			std::replace(answ_strings[i].begin(), answ_strings[i].end(), '\\', ' ');
		}

		m_answer.first.first = answ_strings[0];
		m_answer.first.second =	answ_strings[1];
		m_answer.second.first = std::stoi(answ_strings[2]);
		m_answer.second.second = std::stod(answ_strings[3]);
	}

	if (timer.isNumber()){
		m_cooldown = timer;
	}
}


ActionComponent::~ActionComponent()
{
}
