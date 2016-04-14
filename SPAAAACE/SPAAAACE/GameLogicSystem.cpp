#include "GameLogicSystem.h"


GameLogicSystem::GameLogicSystem()
{
}


GameLogicSystem::~GameLogicSystem()
{
}

void GameLogicSystem::update(Message &postman, std::shared_ptr<GameObject> go, GameLogicComponent &comp, double dt){

	//se base strictement sur l'envoi/réception de messages
	//std::cout << "GameLogic :" << std::to_string((int)go->get<PhysicsComponent>().get()) << " \n";
	if (abs(postman.getMessage("Physics", std::to_string((int)go->get<PhysicsComponent>().get()), MS_COLLISION)) > 0) {

		//std::cout << "HEY";

		double vel = postman.getMessage("Physics", std::to_string((int)go->get<PhysicsComponent>().get()), MS_COLLISION);
		//std::cout << vel;
		postman.addMessage("GameLogic", go->getID(), MS_COLLISION, vel);

		int life_lost = floor(vel / 400.f);
		//std::cout << life_lost << "\n";

		comp.setLife(comp.getCurrentLife() - life_lost);

		postman.addMessage("GameLogic", go->getID(), MS_LIFE_DOWN, life_lost);
	}

	std::cout << dt << "\n";

	if (postman.getMessage("Scene", go->getID(), MS_ENGINE_ACTIVE) > 0){
		//std::cout << "HEY FD\n";
		if (comp.getCurrentFuel() > 0){
			postman.addMessage("GameLogic", go->getID(), MS_ENGINE_ACTIVE, 1);
			double deltaFuel = dt * 800;
			if (comp.getCurrentFuel() - ceil(deltaFuel) < 0){
				deltaFuel = comp.getCurrentFuel();
			}
			comp.setFuel(comp.getCurrentFuel() - ceil(deltaFuel));
		}
		
	}
	else if (comp.getCurrentFuel() < comp.getMaxFuel()){
		//comp.setFuel(comp.getCurrentFuel() + 1);
	}

	if (comp.getCurrentLife() <= 0){
		comp.setLife(0);
		postman.addMessage("GameLogic", go->getID(), MS_DEAD, 1);
	}
}