#include "Scene.h"


Scene::Scene(std::string arg)
{
	//GraphicsSystem::GraphicsSystem();
	
	init(arg);
}


void Scene::init(std::string arg){

	GraphicsSystem::init();
	//GraphicsSystem::reset();

	//initialisation + ouverture script
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, "nem1.lua");
	luain::loadGetKeysFunction(L);


	//AJOUTEZ VOS OBJETS ICI! **********************************************

	//m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock")); 
	//m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock2")); // bon pour le moment c'est batârd d'ajouter 4 fois le "même" objet
	//m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock3")); // mais juste à des positions différentes
	//m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock4")); // ca sera réglé quand on aura une base de création de niveau
	if (arg == "game")
		m_gameObjects.push_back(luain::loadGameObjects(this, L, "player"));// pour le moment ca se code 
	else
		m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock"));


	//À PARTIR DE CE POINT, N'AJOUTEZ PLUS D'OBJETS  ***********************


	///AJOUT BACKGROUND ET MACHINs

	orderByZIndex(); 
	GraphicsSystem::loadBackground("ressources/space_1.png", 0, 250, 200, 255);
	GraphicsSystem::loadBackground("ressources/space_2.png", 1, 150, 100, 150);
	GraphicsSystem::loadBackground("ressources/space_3.png", 2, 200, 220, 255);
	//GraphicsSystem::loadBackground("ressources/bg1.png", 3);

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	m_inSystem.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	m_inSystem.setActionTrigger(AC_START, SDL_SCANCODE_C);
	m_inSystem.setActionTrigger(AC_SELECT, SDL_SCANCODE_D);
	m_inSystem.setActionTrigger(AC_DOWN, SDL_SCANCODE_DOWN);
	m_inSystem.setActionTrigger(AC_LEFT, SDL_SCANCODE_LEFT);
	m_inSystem.setActionTrigger(AC_RIGHT, SDL_SCANCODE_RIGHT);
	m_inSystem.setActionTrigger(AC_HORIZONTAL_PUSH, GP_AXIS_LEFT_JOY_X);
	m_inSystem.setActionTrigger(AC_VERTICAL_PUSH, GP_AXIS_LEFT_JOY_Y);

	std::cout << "END OF INIT\n\n";
	GraphicsSystem::setCameraZoom(1);
}

void Scene::orderByZIndex(){
	int j = 0;
	for (int i(1); i < m_gameObjects.size(); i++){
		j = i - 1;
		while (j >= 0 && m_gameObjects[j]->get<PhysicsComponent>()->getPositionComponent()->getZIndex() > m_gameObjects[i]->get<PhysicsComponent>()->getPositionComponent()->getZIndex()){
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
	m_inSystem.pollInputs();
	postman.clearAll();
	//std::cout << m_inSystem.isJoyConnected() << "\n";
	
	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Scene", "Input", MS_EXIT_REQUEST, 1);

	if (m_inSystem.checkTriggeredAction(AC_START))
		postman.addMessage("game", "Input", 0, 1);

	if (m_inSystem.checkTriggeredAction(AC_SELECT))
		postman.addMessage("menu", "Input", 0, 1);


	GraphicsSystem::initFrame();
	//GraphicsSystem::setCameraAngle(GraphicsSystem::getCameraAngle() - 0.03);
	//GraphicsSystem::setCameraAngle(25);
	//GraphicsSystem::setCameraZoom(GraphicsSystem::getCameraZoom() - 0.00009);
	//GraphicsSystem::setCameraZoom(0.8 + (0.3 * sin(SDL_GetTicks() / 1000.0)));
	//GraphicsSystem::lockCamera(false);
	
	//INPUT GÉNÉRAL
	
	//GraphicsSystem::setCameraZoom(GraphicsSystem::getZoom() + (m_inSystem.checkTriggeredAction(AC_ZOOM) - m_inSystem.checkTriggeredAction(AC_DEZOOM)) / 32768);
	for (int i = 0; i < m_gameObjects.size(); i++){
		//std::cout << "Updating : _" << m_gameObjects[i].getID() << "_\n";
		//on update chaque component de l'objet

		//inputs

		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i]->get<PhysicsComponent>();
		if (pc != nullptr){

			//DÉCISION DE MOUVEMENT : JOUEUR
			if (m_gameObjects[i]->getID() == "player") {
				//on check la direction du joueur
				Vec2 forces(0, 0);
				if (m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) || m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH))
				{
					//std::cout << "HEY";
					forces = Vec2(m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) / 20.0, m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH) / 20.0);
				}
				else {
					if (m_inSystem.checkTriggeredAction(AC_UP))
						forces += Vec2(0, -1564);

					if (m_inSystem.checkTriggeredAction(AC_DOWN))
						forces += Vec2(0, 1564);

					if (m_inSystem.checkTriggeredAction(AC_LEFT))
						forces += Vec2(-1564, 0);

					if (m_inSystem.checkTriggeredAction(AC_RIGHT))
						forces += Vec2(1564, 0);
				}

				pc->setForces(forces);

				double speed = pc->getVelocity().getLength();

				m_phySystem.update(postman, *pc, m_physicsComps, 1.0 / GraphicsSystem::getFPS());
			}

			auto gc = m_gameObjects[i]->get<GraphicsComponent>();
			if (gc != nullptr){

				if (m_gameObjects[i]->getID() == "player"){
					GraphicsSystem::setCameraTarget(gc->getPosition());
				}

				//GraphicsSystem::lockCamera(false);
				GraphicsSystem::update(postman, *gc, 1.0 / GraphicsSystem::getFPS());
			}
		}
		//std::cout << GraphicsSystem::getFPS() << "\n";
		GraphicsSystem::endFrame();
	}
}
