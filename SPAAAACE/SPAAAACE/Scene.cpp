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
		//GraphicsSystem::loadBackground("ressources/space_4.png", 3, 200, 220, 255);
	}

	if (m_id == "menu"){
		m_focusedID = "button_play";
	}

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	//m_inSystem.setActionTrigger(AC_SHOOT, SDL_BUTTON_LEFT);
	m_inSystem.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	m_inSystem.setActionTrigger(AC_START, SDL_SCANCODE_C);
	m_inSystem.setActionTrigger(AC_SELECT, SDL_SCANCODE_D);
	m_inSystem.setActionTrigger(AC_DOWN, SDL_SCANCODE_DOWN);
	m_inSystem.setActionTrigger(AC_LEFT, SDL_SCANCODE_LEFT);
	m_inSystem.setActionTrigger(AC_RIGHT, SDL_SCANCODE_RIGHT);
	m_inSystem.setActionTrigger(AC_HORIZONTAL_PUSH, GP_AXIS_LEFT_JOY_X);
	m_inSystem.setActionTrigger(AC_VERTICAL_PUSH, GP_AXIS_LEFT_JOY_Y);
	m_inSystem.setActionTrigger(AC_SELECT, SDL_SCANCODE_RETURN);
	m_inSystem.setActionTrigger(AC_NEXT, SDL_SCANCODE_SPACE);

	GraphicsSystem::setFont("ressources/CaviarDreams.ttf", 30, { 225, 220, 255 });

	std::cout << "END OF INIT\n\n";
	GraphicsSystem::setCameraZoom(1);
	m_navigationTimer.start();
	m_dialogueTimer.start();
}

void Scene::orderByZIndex(){
	if (m_gameObjects.size() > 1){
		int j = 0;
		bool swapped = false;
		do {
			for (int i(0); i < m_gameObjects.size() - 1; i++){

				if (m_gameObjects[i]->get<PositionComponent>()->getZIndex() < m_gameObjects[i + 1]->get<PositionComponent>()->getZIndex()){
					std::iter_swap(m_gameObjects.begin() + i, m_gameObjects.begin() + i + 1);
					swapped = true;
				}

				if (i >= m_gameObjects.size() - 2 && !swapped){
					swapped = true;
				}
			}
		} while (!swapped);

		std::reverse(m_gameObjects.begin(), m_gameObjects.end());

		//on place le joueur au "dessus" des objets à sa même hauteur
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
}

Scene::~Scene()
{
	//GraphicsSystem::close();
}

void Scene::update(Message &postman)
{
	postman.clearAll();
	//INPUT GÉNÉRAL
	m_inSystem.pollInputs();


	//GESTION DU MENU

	//NAVIGATION AVEC FLÈCHES
	if (m_id == "menu"){

		//en attendant gestion press/release
		if (m_navigationTimer.getTicks() > 200){

			//création du "template" de menu
			std::vector<std::string> menuList;
			menuList.push_back("button_play");
			menuList.push_back("button_quit");

			int pos = std::find(menuList.begin(), menuList.end(), m_focusedID) - menuList.begin();

			int newPos = pos;
			if (m_inSystem.checkTriggeredAction(AC_DOWN)){
				//on avance vers le bas 
				newPos = (pos + 1) % menuList.size();
			}
			else if (m_inSystem.checkTriggeredAction(AC_UP)){
				//on recule vers le haut
				newPos = (pos - 1) % menuList.size();
			}

			if (m_focusedID != menuList[newPos]){
				m_navigationTimer.stop();
				m_navigationTimer.start();
				m_focusedID = menuList[newPos];
			}
		}

	}

	if (m_inSystem.checkTriggeredAction(AC_SELECT)){
		postman.addMessage("Scene",  m_focusedID, MS_SELECT, 1);
	}

	if (m_inSystem.checkTriggeredAction(AC_NEXT)) {
		if (m_dialogueTimer.getTicks() > 575) {
			postman.addMessage("Scene", "Input", MS_DIALOGUE_NEXT, 1); 
			m_dialogueTimer.stop();
			m_dialogueTimer.start();
		}
	}

	if (m_inSystem.checkTriggeredAction(AC_START)) {
		
		GraphicsSystem::print("HEY it's started mate!");
		GraphicsSystem::print("What are ye doin?");
		std::cout << "ADD\n";
	}

	//TIR DU VAISSEAU

	//=======================

	//ashdgahds


	GraphicsSystem::initFrame();
	
	for (int i = 0; i < m_gameObjects.size(); i++){

		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i]->get<PhysicsComponent>();
		if (pc != nullptr){

			//DÉCISION DE MOUVEMENT : JOUEUR
			if (m_gameObjects[i]->getID() == "player") {

				if (m_gameObjects[i]->get<GameLogicComponent>()->getCurrentFuel() > 0) { // si assez de fuel
					//on check la direction du joueur
					Vec2 forces(0, 0);
					Vec2 direction(cos(pc->getPositionComponent()->getAngle() * (3.14159 / 180.0)), sin(pc->getPositionComponent()->getAngle() * (3.14159 / 180.0)));
					int pwr = m_gameObjects[i]->get<GameLogicComponent>()->getEnginePower();

					double angle = 0;

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

					//std::cout << forces.getNormalized().y() << " ";
					angle = forces.getAngle();
					//std::cout << angle << "\n";

					//gestion avec angle
					/*	double deltaAngle = 0;

					if (m_inSystem.checkTriggeredAction(AC_UP))
						forces += direction * pwr;
					if (m_inSystem.checkTriggeredAction(AC_DOWN))
						forces += pc->getVelocity().getNormalized() * -pwr;
					
					if (m_inSystem.checkTriggeredAction(AC_LEFT))
						deltaAngle = -0.95;
					
					if (m_inSystem.checkTriggeredAction(AC_RIGHT))
						deltaAngle = 0.95;


					if(deltaAngle == 0) //réduction automatique de la rotation
						deltaAngle = -(pc->getAngularVelocity() / 200.0);*/

					//on baisse le fuel

					if (forces.getLength() > 0) {

						postman.addMessage("Scene-", m_gameObjects[i]->getID(), MS_ENGINE_ACTIVE, 1);
						pc->getPositionComponent()->setAngle(angle);
					}

					pc->setForces(forces);
					
				}
			}
			
			m_phySystem.update(postman, *pc, m_physicsComps, 1.0 / GraphicsSystem::getFPS());

		}


		auto GLC = m_gameObjects[i]->get<GameLogicComponent>();
		if (GLC != nullptr){
			GameLogicSystem::update(postman, m_gameObjects[i], *GLC);
		}

		auto AC = m_gameObjects[i]->get<ActionComponent>();
		if (AC != nullptr){
			ActionSystem::update(postman, *AC);
		}

		auto gc = m_gameObjects[i]->get<GraphicsComponent>();
		if (gc != nullptr){

			if (m_gameObjects[i]->getID() == m_focusedID){
				gc->getSprite()->setSpriteSheet("selected");
			}
			else {
				gc->getSprite()->setSpriteSheet("default");
			}

			if (m_gameObjects[i]->getID() == "player"){

				double vel = m_gameObjects[i]->get<PhysicsComponent>()->getVelocity().getLength();
				double zoom = 0.625;

				//zoom min: 0.625 (valeur arbitraire)

				/*if (vel < 2500)
					zoom = 0.625 + (0.3 - (0.3* vel / 2500));*/

				//std::cout << vel <<  "vs " << zoom << "\n";

				GraphicsSystem::setCameraZoom(zoom);
				GraphicsSystem::setCameraTarget(gc->getPosition());

				// et on update le hud en fonction du joueur

				for (int j = 0; j < m_gameObjects.size(); j++){
					if (m_gameObjects[j]->getID().find("hud") != std::string::npos) { // si l'objet fait partie du hud

						std::string id = m_gameObjects[j]->getID();

						if (id == "hud_fuel"){
							//if (postman.getMessage("GameLogic", m_gameObjects[i]->getID(), MS_ENGINE_ACTIVE) > 0){

								//on réduit le fuel
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
		postman.addMessage("Action", "Button", MS_EXIT_REQUEST, 1);

	GraphicsSystem::endFrame(postman);
}
