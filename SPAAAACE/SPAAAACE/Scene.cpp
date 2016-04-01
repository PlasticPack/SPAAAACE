#include "Scene.h"


Scene::Scene(std::string arg, std::string id)
{
	m_id = id;
	init(arg);
}


void Scene::init(std::string arg){

	GraphicsSystem::init();

	//AJOUTEZ VOS OBJETS ICI! **********************************************


	try {
		luain::loadFromRep(this, m_gameObjects, arg);
	}
	catch (std::exception e){
		std::cout << e.what() << "\n";

	}

	//� PARTIR DE CE POINT, N'AJOUTEZ PLUS D'OBJETS  ***********************


	///AJOUT BACKGROUND ET MACHINs

	orderByZIndex(); 
	if (m_id == "game"){ // pour le moment c'est ca
		GraphicsSystem::loadBackground("ressources/space_1.png", 0, 250, 200, 255);
		GraphicsSystem::loadBackground("ressources/space_2.png", 1, 150, 100, 150);
		GraphicsSystem::loadBackground("ressources/space_3.png", 2, 200, 220, 255);
		//GraphicsSystem::loadBackground("ressources/space_4.png", 3, 200, 220, 255);
	}

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	m_inSystem.setActionTrigger(AC_SHOOT, SDL_BUTTON_LEFT);
	m_inSystem.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	m_inSystem.setActionTrigger(AC_START, SDL_SCANCODE_C);
	m_inSystem.setActionTrigger(AC_SELECT, SDL_SCANCODE_D);
	m_inSystem.setActionTrigger(AC_DOWN, SDL_SCANCODE_DOWN);
	m_inSystem.setActionTrigger(AC_LEFT, SDL_SCANCODE_LEFT);
	m_inSystem.setActionTrigger(AC_RIGHT, SDL_SCANCODE_RIGHT);
	m_inSystem.setActionTrigger(AC_HORIZONTAL_PUSH, GP_AXIS_LEFT_JOY_X);
	m_inSystem.setActionTrigger(AC_VERTICAL_PUSH, GP_AXIS_LEFT_JOY_Y);

	GraphicsSystem::setFont("ressources/CaviarDreams.ttf", 30, { 225, 220, 255 });

	std::cout << "END OF INIT\n\n";
	GraphicsSystem::setCameraZoom(1);
}

void Scene::orderByZIndex(){
	int j = 0;
	bool swapped = false;
	do {
		for (int i(1); i < m_gameObjects.size() - 1; i++){

			if (m_gameObjects[i]->get<PositionComponent>()->getZIndex() < m_gameObjects[i+1]->get<PositionComponent>()->getZIndex()){
				std::iter_swap(m_gameObjects.begin() + i, m_gameObjects.begin() + i + 1);
				swapped = true;
			}
		}
	} while (!swapped);

	std::reverse(m_gameObjects.begin(), m_gameObjects.end());

	//on place le joueur au "dessus" des objets � sa m�me hauteur
	for (int i = 0; i < m_gameObjects.size(); i++){
		if (m_gameObjects[i]->getID() == "player"){
			if (i != m_gameObjects.size() - 1) {
				for (int j = i; j < m_gameObjects.size(); j++){
					if (m_gameObjects[j]->getID() != "player" && m_gameObjects[j]->get<PositionComponent>()->getZIndex() == m_gameObjects[i]->get<PositionComponent>()->getZIndex()) {
						std::iter_swap(m_gameObjects.begin() + i, m_gameObjects.begin() + j);
						std::cout << i << j << "\n";
						break;
					}
				}
			}
		}
	}
}

Scene::~Scene()
{
	//GraphicsSystem::close();
}

void Scene::update(Message &postman)
{
	postman.clearAll();
	//INPUT G�N�RAL
	m_inSystem.pollInputs();

	/*if (m_inSystem.checkTriggeredAction(AC_START))
		postman.addMessage("game", "Input", 0, 1);

	if (m_inSystem.checkTriggeredAction(AC_SELECT))
		postman.addMessage("menu", "Input", 0, 1);*/

	if (m_inSystem.checkTriggeredAction(AC_SHOOT))
		postman.addMessage("game", "Input", MS_SHOOT, 1);


	//TIR DU VAISSEAU

	//=======================




	GraphicsSystem::initFrame();
	
	for (int i = 0; i < m_gameObjects.size(); i++){

		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i]->get<PhysicsComponent>();
		if (pc != nullptr){

			//D�CISION DE MOUVEMENT : JOUEUR
			if (m_gameObjects[i]->getID() == "player") {

				if (m_gameObjects[i]->get<GameLogicComponent>()->getCurrentFuel() > 0) { // si assez de fuel
					//on check la direction du joueur
					Vec2 forces(0, 0);
					int pwr = m_gameObjects[i]->get<GameLogicComponent>()->getEnginePower();


					if (m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) || m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH))
					{
						forces = Vec2(m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) / 20.0, m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH) / 20.0);
					}
					else {
						if (m_inSystem.checkTriggeredAction(AC_UP))
							forces += Vec2(0, -pwr);

						if (m_inSystem.checkTriggeredAction(AC_DOWN))
							forces += Vec2(0, pwr);

						if (m_inSystem.checkTriggeredAction(AC_LEFT))
							forces += Vec2(-pwr, 0);

						if (m_inSystem.checkTriggeredAction(AC_RIGHT))
							forces += Vec2(pwr, 0);
					}

					//on baisse le fuel

					if (forces.getLength() > 0)
						postman.addMessage("Scene", m_gameObjects[i]->getID(), MS_ENGINE_ACTIVE, 1);


					pc->setForces(forces);

					double speed = pc->getVelocity().getLength();
				}
			}
			
			m_phySystem.update(postman, *pc, m_physicsComps, 1.0 / GraphicsSystem::getFPS());

		}


		auto GLC = m_gameObjects[i]->get<GameLogicComponent>();
		if (GLC != nullptr){
			GameLogicSystem::update(postman, m_gameObjects[i], *GLC);
		}



		auto gc = m_gameObjects[i]->get<GraphicsComponent>();
		if (gc != nullptr){

			if (m_gameObjects[i]->getID() == "player"){

				double vel = m_gameObjects[i]->get<PhysicsComponent>()->getVelocity().getLength();
				double zoom = 0.625;

				//zoom min: 0.625 (valeur arbitraire)

				if (vel < 2500)
					zoom = 0.625 + (0.3 - (0.3* vel / 2500));

				//std::cout << vel <<  "vs " << zoom << "\n";

				GraphicsSystem::setCameraZoom(zoom);
				GraphicsSystem::setCameraTarget(gc->getPosition());

				// et on update le hud en fonction du joueur

				for (int j = 0; j < m_gameObjects.size(); j++){
					if (m_gameObjects[j]->getID().find("hud") != std::string::npos) { // si l'objet fait partie du hud

						std::string id = m_gameObjects[j]->getID();

						if (id == "hud_fuel"){
							//if (postman.getMessage("GameLogic", m_gameObjects[i]->getID(), MS_ENGINE_ACTIVE) > 0){

								//on r�duit le fuel
								Vec2 baseSize = m_gameObjects[j]->get<GraphicsComponent>()->getMaxSize();
								double fuel = (double)(m_gameObjects[i]->get<GameLogicComponent>()->getCurrentFuel()) / (double)m_gameObjects[i]->get<GameLogicComponent>()->getMaxFuel();
								m_gameObjects[j]->get<GraphicsComponent>()->setSize(Vec2(baseSize.x() * fuel, baseSize.y()));

							//}
						}
						else if (id == "hud_life"){
							//if (postman.getMessage("GameLogic", m_gameObjects[i]->getID(), MS_LIFE_DOWN) > 0) {

								Vec2 baseSize = m_gameObjects[j]->get<GraphicsComponent>()->getMaxSize();
								double life = (double)(m_gameObjects[i]->get<GameLogicComponent>()->getCurrentLife()) / (double)m_gameObjects[i]->get<GameLogicComponent>()->getMaxLife();
								m_gameObjects[j]->get<GraphicsComponent>()->setSize(Vec2(baseSize.x() * life, baseSize.y()));
							//}
						}
					}
				}

			}
			if (postman.getMessage("GameLogic", m_gameObjects[i]->getID(), MS_COLLISION) > 0) {
			}
			GraphicsSystem::update(postman, m_gameObjects[i]->getID(),*gc, 1.0 / GraphicsSystem::getFPS());
		}

	}
	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Scene", "Input", MS_EXIT_REQUEST, 1);

	GraphicsSystem::endFrame();
}
