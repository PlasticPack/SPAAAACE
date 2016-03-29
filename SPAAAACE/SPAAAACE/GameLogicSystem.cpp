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
		//std::cout << vel;
		postman.addMessage("GameLogic", "Collision", MS_COLLISION, vel);


		int deltaLife = 3;
		postman.addMessage("GameLogic", "Life", MS_LIFE_DOWN, deltaLife);

		//on baisse la vie
		//comp.setLife(comp.getCurrentLife() - vel);
		postman.deleteMessage("Physics", "Physics");
	}

	if (postman.getMessage("player", "player", MS_FUEL_DOWN) > 0){
		//std::cout << "HEY FD\n";
		postman.addMessage("GameLogic", "Fuel", MS_FUEL_DOWN, 1);
		
		
		postman.deleteMessage("player", "player");
	}

}