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

	//À PARTIR DE CE POINT, N'AJOUTEZ PLUS D'OBJETS  ***********************


	///AJOUT BACKGROUND ET MACHINs

	orderByZIndex(); 
	if (m_id == "game"){ // pour le moment c'est ca
		GraphicsSystem::loadBackground("ressources/space_1.png", 0, 250, 200, 255);
		GraphicsSystem::loadBackground("ressources/space_2.png", 1, 150, 100, 150);
		GraphicsSystem::loadBackground("ressources/space_3.png", 2, 200, 220, 255);
		//GraphicsSystem::loadBackground("ressources/bg1.png", 3);
	}

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	m_inSystem.setActionTrigger(AC_SHOOT, SDL_SCANCODE_SPACE);
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
	for (int i(1); i < m_gameObjects.size(); i++){
		j = i - 1;

		while (j >= 0 && m_gameObjects[j]->get<PositionComponent>()->getZIndex() > m_gameObjects[i]->get<PositionComponent>()->getZIndex()){
			std::iter_swap(m_gameObjects.begin() + j + 1, m_gameObjects.begin() + j);
			j -= 1;
		}
	}
}

Scene::~Scene()
{
	//GraphicsSystem::close();
}

void Scene::update(Message &postman)
{
	//postman.clearAll();
	//INPUT GÉNÉRAL
	m_inSystem.pollInputs();
	postman.clearAll();
	/*if (m_inSystem.checkTriggeredAction(AC_START))
		postman.addMessage("game", "Input", 0, 1);

	if (m_inSystem.checkTriggeredAction(AC_SELECT))
		postman.addMessage("menu", "Input", 0, 1);*/

	if (m_inSystem.checkTriggeredAction(AC_SHOOT))
	{
		postman.addMessage("game", "Input", MS_SHOOT, 1);
	}

	
	//TIR DU VAISSEAU

	//=======================




	GraphicsSystem::initFrame();
	
	for (int i = 0; i < m_gameObjects.size(); i++){

		//std::cout << ".";


		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i]->get<PhysicsComponent>();
		if (pc != nullptr){

			//DÉCISION DE MOUVEMENT : JOUEUR
			if (m_gameObjects[i]->getID() == "player") {
				//on check la direction du joueur
				Vec2 forces(0, 0);
				
				if (m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) || m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH))
				{
					forces = Vec2(m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) / 20.0, m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH) / 20.0);
					
				}
				else {
					if (m_inSystem.checkTriggeredAction(AC_UP))
					{
						forces += Vec2(0, -1564);
					
					}

					if (m_inSystem.checkTriggeredAction(AC_DOWN))
					{
						forces += Vec2(0, 1564);
					
					}

					if (m_inSystem.checkTriggeredAction(AC_LEFT))
					{
						forces += Vec2(-1564, 0);
					
					}

					if (m_inSystem.checkTriggeredAction(AC_RIGHT))
					{
						forces += Vec2(1564, 0);
					
					}
					
				}

				//on baisse le fuel
				
				if(forces.getLength() > 0)
					postman.addMessage("player", "player", MS_ENGINE_ACTIVE, 1);


				pc->setForces(forces);

				double speed = pc->getVelocity().getLength();
			}
			
			m_phySystem.update(postman, *pc, m_physicsComps, 1.0 / GraphicsSystem::getFPS());

		}

		
		auto GLC = m_gameObjects[i]->get<GameLogicComponent>();
		if (GLC != nullptr){
			GameLogicSystem::update(postman, *GLC);

		}


		auto gc = m_gameObjects[i]->get<GraphicsComponent>();
		if (gc != nullptr){

			if (m_gameObjects[i]->getID() == "player"){
				GraphicsSystem::setCameraTarget(gc->getPosition());

				// et on update le hud en fonction du joueur

				for (int j = 0; j < m_gameObjects.size(); j++){
					if (m_gameObjects[j]->getID().find("hud") != std::string::npos) { // si l'objet fait partie du hud

						std::string id = m_gameObjects[j]->getID();

						if (id == "hud_fuel"){
							if (postman.getMessage("GameLogic", "Fuel", MS_ENGINE_ACTIVE) > 0){

								//on réduit le fuel
								m_gameObjects[j]->get<GraphicsComponent>()->setSize(m_gameObjects[j]->get<GraphicsComponent>()->getSize() - Vec2(0.1,0));
								//std::cout << m_gameObjects[j]->get<GraphicsComponent>()->getSize().x() << "\n";

								
								GraphicsSystem::print("vroum vroum vroum :) ");
							}
						}
						else if (id == "hud_life"){

						}
					}
				}

			}
			if (postman.getMessage("GameLogic", "Collision", MS_COLLISION) > 0) {
				//std::cout << "Bang.";
				GraphicsSystem::printAt("COLLISION!", 0, 0, 400, 200);
				//SDL_Delay(100);
				
			}
			GraphicsSystem::update(postman, *gc, 1.0 / GraphicsSystem::getFPS());
		}

	}

	

	//MusSystem
	m_musSytem.update(postman);

	//GraphicsSystem::print("S.O.S");


	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Scene", "Input", MS_EXIT_REQUEST, 1);

	GraphicsSystem::endFrame();
}
