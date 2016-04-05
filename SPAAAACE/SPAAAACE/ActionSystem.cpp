#include "ActionSystem.h"


void ActionSystem::update(Message& postman, ActionComponent& ac){

	if (postman.getMessage(ac.getTrigger().first.first, ac.getTrigger().first.second, ac.getTrigger().second.first) == ac.getTrigger().second.second){
		std::cout << "TRIGGERED";
		postman.addMessage(ac.getAnswer().first.first, ac.getAnswer().first.second, ac.getAnswer().second.first, ac.getAnswer().second.second);
	}

}