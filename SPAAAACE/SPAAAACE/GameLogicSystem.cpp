#include "GameLogicSystem.h"


GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}

void GameLogicSystem::update(Message &postman, GameLogicComponent &comp){

	//se base strictement sur l'envoi/réception de messages
	
	if (postman.getMessage("Physics", "Physics", MS_COLLISION) > 0) {

		double vel = postman.getMessage("Physics", "Physics", MS_COLLISION);
		postman.addMessage("GameLogic", "GameLogic", MS_COLLISION, vel);

		postman.deleteMessage("Physics", "Physics");

		int deltaLife = 3;
		postman.addMessage("GameLogic", "GameLogic", MS_LIFE_DOWN, deltaLife);

		//on baisse la vie
		//comp.setLife(comp.getCurrentLife() - vel);
	}

	if (postman.getMessage("player", "player", MS_FUEL_DOWN) > 0){
		//std::cout << "HEY FD\n";
		postman.addMessage("GameLogic", "GameLogic", MS_FUEL_DOWN, 1);
		
		
		
		
		postman.deleteMessage("player", "player");
	}

}