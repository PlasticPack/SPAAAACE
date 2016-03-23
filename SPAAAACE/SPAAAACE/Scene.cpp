#include "Scene.h"


Scene::Scene()
{
	//GraphicsSystem::GraphicsSystem();
	
	init();
}


void Scene::init(){

	GraphicsSystem::init();

	//initialisation + ouverture script
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, "nem1.lua");
	luain::loadGetKeysFunction(L);


	//AJOUTEZ VOS OBJETS ICI! **********************************************

	m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock")); 
	m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock2")); // bon pour le moment c'est batârd d'ajouter 4 fois le "même" objet
	m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock3")); // mais juste à des positions différentes
	m_gameObjects.push_back(luain::loadGameObjects(this, L, "rock4")); // ca sera réglé quand on aura une base de création de niveau
	m_gameObjects.push_back(luain::loadGameObjects(this, L, "player"));// pour le moment ca se code 



	//À PARTIR DE CE POINT, N'AJOUTEZ PLUS D'OBJETS  ***********************


	///AJOUT BACKGROUND ET MACHINs

	orderByZIndex(); 
	GraphicsSystem::loadBackground("ressources/space_1.png", 0, 150, 100, 100);
	GraphicsSystem::loadBackground("ressources/space_2.png", 1, 150, 100, 150);
	GraphicsSystem::loadBackground("ressources/space_3.png", 2, 200, 220, 255);
	//GraphicsSystem::loadBackground("ressources/bg1.png", 3);

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	m_inSystem.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	m_inSystem.setActionTrigger(AC_DOWN, SDL_SCANCODE_DOWN);
	m_inSystem.setActionTrigger(AC_LEFT, SDL_SCANCODE_LEFT);
	m_inSystem.setActionTrigger(AC_RIGHT, SDL_SCANCODE_RIGHT);
	m_inSystem.setActionTrigger(AC_HORIZONTAL_PUSH, GP_AXIS_LEFT_JOY_X);
	m_inSystem.setActionTrigger(AC_VERTICAL_PUSH, GP_AXIS_LEFT_JOY_Y);
	m_inSystem.setActionTrigger(AC_DEZOOM, GP_AXIS_RT);
	m_inSystem.setActionTrigger(AC_ZOOM, GP_AXIS_LT);

	std::cout << "END OF INIT\n\n";
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
/*
void Scene::addSkyBody(double x, double y, double mass, std::string id, Uint8 r, Uint8 g, Uint8 b){

	std::string file = "ressources/skyBody.png";
	if (id == "player"){
		file = "ressources/test.png";
	}
	Sprite skyBodySprite(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture(file, r, g, b), 2, 2));

	if (id == "player"){
		skyBodySprite.addSpriteSheet("collision", std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture("ressources/test_coll.png"), 2, 2));
	}

	m_posComps.push_back(std::make_shared<PositionComponent>());
	
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps.back())); // on ajoute le dernier
	m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>(skyBodySprite)));

	m_gameObjects.push_back(GameObject(id));
	m_gameObjects.back().addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps.back());
	m_gameObjects.back().addComponent(std::type_index(typeid(GraphicsComponent)), m_graphicsComps.back());
	//m_gameObjects.back().addComponent(std::type_index(typeid(InputsComponent)), m_inputsComps.back());

	m_gameObjects.back().get<PhysicsComponent>()->setPosition(Vec2(x, y));
	m_gameObjects.back().get<PhysicsComponent>()->setMass(mass);

	if (id == "player"){
		m_gameObjects.back().get<PhysicsComponent>()->setHitboxRadius(50);
	}

	//m_gameObjects.back().get<GraphicsComponent>()->setSprite(Sprite(SpriteSheet(GraphicsSystem::loadTexture("test.png"), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 })));
	std::cout << "added skyBody\n";
}*/

Scene::~Scene()
{
	GraphicsSystem::close();
}

void Scene::update(Message &postman)
{
	m_inSystem.pollInputs();
	postman.clearAll();
	//std::cout << m_inSystem.isJoyConnected() << "\n";
	
	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Scene", "Input", MS_EXIT_REQUEST, 1);


	GraphicsSystem::initFrame();
	GraphicsSystem::setCameraZoom(1);
	//GraphicsSystem::lockCamera(false);
	
	//INPUT GÉNÉRAL
	
	GraphicsSystem::setCameraZoom(GraphicsSystem::getZoom() + (m_inSystem.checkTriggeredAction(AC_ZOOM) - m_inSystem.checkTriggeredAction(AC_DEZOOM)) / 32768);
	for (int i = 0; i < m_gameObjects.size(); i++){
		//std::cout << "Updating : _" << m_gameObjects[i].getID() << "_\n";
		//on update chaque component de l'objet

		//inputs

		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i]->get<PhysicsComponent>();
		if (pc != nullptr){
		
			if (m_gameObjects[i]->getID() == "player") {
			//on check la direction du joueur
				Vec2 forces(0, 0);
				if (m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH) || m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH))
				{
					forces = Vec2(m_inSystem.checkTriggeredAction(AC_HORIZONTAL_PUSH), m_inSystem.checkTriggeredAction(AC_VERTICAL_PUSH));
				}
				else
				{

					if (m_inSystem.checkTriggeredAction(AC_UP))
						forces += Vec2(0, -5640);

					if (m_inSystem.checkTriggeredAction(AC_DOWN))
						forces += Vec2(0, 5640);

					if (m_inSystem.checkTriggeredAction(AC_LEFT))
						forces += Vec2(-5640, 0);

					if (m_inSystem.checkTriggeredAction(AC_RIGHT))
						forces += Vec2(5640, 0);
				}

				pc->setForces(forces);

				double speed = pc->getVelocity().getLength();
				
				//std::cout << GraphicsSystem::getZoom()<<  "  /  " << speed << "\n";

				/*if (GraphicsSystem::getZoom() >= 0.05  && GraphicsSystem::getZoom() <= 0.3) {
					GraphicsSystem::setCameraZoom((25000 / (speed + 1)) / 10);
					if (GraphicsSystem::getZoom() < 0.05)
						GraphicsSystem::setCameraZoom(0.15);
					if (GraphicsSystem::getZoom() > 0.3)
						GraphicsSystem::setCameraZoom(0.3);
				}*/
			}
			
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
