#include "ActionSystem.h"


void ActionSystem::update(Message& postman, ActionComponent& ac){

	if (!ac.isOn() && ac.getTimer().getTicks() > ac.getCooldown()){
		//std::cout << "READY!";
		ac.set(true);
	}

	if (ac.isOn()){
		if (postman.getMessage(ac.getTrigger().first.first, ac.getTrigger().first.second, ac.getTrigger().second.first) >= ac.getTrigger().second.second){ // pour le moment >= pour true/false

			postman.addMessage(ac.getAnswer().first.first, ac.getAnswer().first.second, ac.getAnswer().second.first, ac.getAnswer().second.second);
			ac.set(false);
			ac.resetTimer();
		}
	}
}