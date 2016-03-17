#include "Scene.h"


Scene::Scene()
{
	//GraphicsSystem::GraphicsSystem();
	
	init();
}


void Scene::init(){

	GraphicsSystem::init();

	//ajout des étoiles en background
	srand(time(NULL));
	/*for (int i = 0; i < 75; i++){
		addSkyBody(rand() % 3000 + 500, rand() % 3000 + 500, rand() % 10 + 15, "shootingStar_" + std::to_string(m_gameObjects.size()), rand() % 230 + 15, rand() % 230 + 15);
		//m_gameObjects.back().get<PhysicsComponent>()->activate(false);
		m_gameObjects.back().get<PhysicsComponent>()->setVelocity(Vec2(rand() % 820 - 75, rand() % 400 - 25));
		//m_gameObjects.back().get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.9);
	}

	addSkyBody(180, 110, 1, "skyBody_" + std::to_string(m_gameObjects.size()));

	m_gameObjects.back().get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.3);

	addSkyBody(0, 0, 8000, "skyBody_" + std::to_string(m_gameObjects.size()));

	m_gameObjects.back().get<PhysicsComponent>()->setHitboxRadius(700);
	m_gameObjects.back().get<GraphicsComponent>()->setSize(Vec2(1400, 1400));
	m_gameObjects.back().get<GraphicsComponent>()->setAnimationSpeed(3);

	addSkyBody(800, 800, 16, "skyBody_" + std::to_string(m_gameObjects.size()), 30, 200, 200);
	addSkyBody(-800, 8000, 0.01, "skyBody_" + std::to_string(m_gameObjects.size()));
	addSkyBody(-900, -900, 10, "skyBody_" + std::to_string(m_gameObjects.size()));*/

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, "nem1.lua");
	luain::loadGetKeysFunction(L);


	m_gameObjects.push_back(*luain::loadGameObjects(this, L, "player"));
	//m_graphicsComps.back()->setSprite(std::make_shared<Sprite>(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture("ressources/test.png"), 2, 2)));

	/*m_posComps.push_back(m_gameObjects.back().get<PositionComponent>());
	m_physicsComps.push_back(m_gameObjects.back().get<PhysicsComponent>());
	m_physicsComps.back()->setPositionComp(m_posComps.back());

	m_graphicsComps.push_back(m_gameObjects.back().get<GraphicsComponent>());
	m_graphicsComps.back()->setPositionComponent(m_posComps.back());
	m_physicsComps.push_back(m_gameObjects.back().get<PhysicsComponent>());

	/*Sprite skyBodySprite(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture("ressources/snail.png"), 3));
	m_posComps.push_back(std::make_shared<PositionComponent>());
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>( "!mahciN" ,m_posComps.back())); // on ajoute le dernier
	m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>(skyBodySprite)));
	

	std::cout << m_physicsComps.size() << "\n";

	//AJOUT JOUEUR
	
	addSkyBody(0, 0, 1.1, "player");
	m_gameObjects.back().get<PhysicsComponent>()->setVelocity(Vec2(0, -60));
	m_gameObjects.back().get<GraphicsComponent>()->setSize(Vec2(100, 100));
	m_gameObjects.back().get<GraphicsComponent>()->setAnimationSpeed(15);*/


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

	std::cout << "END OF INIT\n\n";
}

void Scene::orderByZIndex(){
	int j = 0;
	for (int i(1); i < m_gameObjects.size(); i++){
		GameObject x = m_gameObjects[i];
		j = i - 1;
		while (j >= 0 && m_gameObjects[j].get<PhysicsComponent>()->getPositionComponent()->getZIndex() > x.get<PhysicsComponent>()->getPositionComponent()->getZIndex()){
			m_gameObjects[j + 1] = m_gameObjects[j];
			j -= 1;
		}
		m_gameObjects[j + 1] = x;
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
	GraphicsSystem::setCameraZoom(0.3);
	//GraphicsSystem::lockCamera(false);
	
	for (int i = 0; i < m_gameObjects.size(); i++){

		//on update chaque component de l'objet
		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i].get<PhysicsComponent>();
		if (pc != nullptr){

			if (m_gameObjects[i].getID() == "player_copy") {
			//on check la direction du joueur
				Vec2 forces(0, 0);
				if (m_inSystem.checkTriggeredAction(AC_UP))
					forces += Vec2(0, -11640);

				if (m_inSystem.checkTriggeredAction(AC_DOWN))
					forces += Vec2(0, 11640);

				if (m_inSystem.checkTriggeredAction(AC_LEFT))
					forces += Vec2(-11640, 0);

				if (m_inSystem.checkTriggeredAction(AC_RIGHT))
					forces += Vec2(11640, 0);

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
			
			// m_components[std::type_index(typeid(PhysicsComponent))]
			m_phySystem.update(postman, *pc, m_physicsComps, 1.0 / GraphicsSystem::getFPS());
		}

		auto gc = m_gameObjects[i].get<GraphicsComponent>();
		if (gc != nullptr){

			if (m_gameObjects[i].getID() == "player_copy"){
				GraphicsSystem::setCameraTarget(gc->getPosition());
			}

			//GraphicsSystem::lockCamera(false);
			GraphicsSystem::update(postman, *gc, 1.0 / GraphicsSystem::getFPS());
		}
	}
	//std::cout << GraphicsSystem::getFPS() << "\n";
	GraphicsSystem::endFrame();
}
