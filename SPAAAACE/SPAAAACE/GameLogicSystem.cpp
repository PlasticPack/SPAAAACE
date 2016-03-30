#include "GameLogicSystem.h"


GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}

void GameLogicSystem::update(Message &postman, std::string id, GameLogicComponent &comp){

	//se base strictement sur l'envoi/réception de messages
	
	if (abs(postman.getMessage("Physics", "CollisionResolve", MS_COLLISION)) > 0) {

		//std::cout << "HEY";

		double vel = postman.getMessage("Physics", "CollisionResolve", MS_COLLISION);
		//std::cout << vel;
		postman.addMessage("GameLogic", id, MS_COLLISION, vel);

		int life_lost = floor(vel / 100.f);
		std::cout << life_lost << "\n";

		comp.setLife(comp.getCurrentLife() - life_lost);

		postman.addMessage("GameLogic", id, MS_LIFE_DOWN, life_lost);
	}

	if (postman.getMessage("Scene", id, MS_ENGINE_ACTIVE) > 0){
		//std::cout << "HEY FD\n";
		postman.addMessage("GameLogic", id, MS_ENGINE_ACTIVE, 1);
		comp.setFuel(comp.getCurrentFuel());
		
	}

	if (comp.getCurrentLife() <= 0){
		comp.setLife(0);
		postman.addMessage("GameLogic", id, MS_DEAD, 1);
	}
}