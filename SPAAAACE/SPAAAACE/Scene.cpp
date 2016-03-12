#include "Scene.h"


Scene::Scene()
{
	init();
}


void Scene::init(){
	//ajout des étoiles en background
	srand(time(NULL));
	/*for (int i = 0; i < 25; i++){
		addSkyBody(rand() % 10000 - 5000, rand() % 10000 - 5000, rand() % 100 + 150, "shootingStar_" + std::to_string(m_gameObjects.size()), rand() % 100 + 150, rand() % 100 + 150);
		m_gameObjects.back().get<PhysicsComponent>()->activate(false);
		m_gameObjects.back().get<PhysicsComponent>()->setVelocity(Vec2(rand() % 1200 - 765, rand() % 400 - 205));
		m_gameObjects.back().get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.09);
	}*/

	/*addSkyBody(180, 110, 1, "skyBody_" + std::to_string(m_gameObjects.size()));

	m_gameObjects.back().get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.3);*/

	addSkyBody(0, 0, 100, "skyBody_" + std::to_string(m_gameObjects.size()));

	m_gameObjects.back().get<PhysicsComponent>()->setHitboxRadius(700);
	m_gameObjects.back().get<GraphicsComponent>()->setSize(Vec2(1400, 1400));
	m_gameObjects.back().get<GraphicsComponent>()->setAnimationSpeed(3);

	//addSkyBody(800, 800, 16, "skyBody_" + std::to_string(m_gameObjects.size()), 30, 200, 200);
	//addSkyBody(-800, 8000, 0.01, "skyBody_" + std::to_string(m_gameObjects.size()));
	//addSkyBody(-900, -900, 10, "skyBody_" + std::to_string(m_gameObjects.size()));
	
	addSkyBody(1364, 1364, 0.1, "player");
	m_gameObjects.back().get<PhysicsComponent>()->setVelocity(Vec2(0, -60));
	m_gameObjects.back().get<GraphicsComponent>()->setSize(Vec2(100, 100));
	m_gameObjects.back().get<GraphicsComponent>()->setAnimationSpeed(15);

	std::cout << "END OF INIT\n\n";

	m_gameObjects.back().get<GraphicsComponent>()->setHaloColor({ 150, 200, 255, 255 });

	orderByZIndex();
	m_graSystem.loadBackground("ressources/space_1.png", 0, 150, 100, 100);
	m_graSystem.loadBackground("ressources/space_2.png", 1, 150, 100, 150);
	m_graSystem.loadBackground("ressources/space_3.png", 2, 200, 220, 255);
	//m_graSystem.loadBackground("ressources/bg1.png", 3);

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	m_inSystem.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	m_inSystem.setActionTrigger(AC_DOWN, SDL_SCANCODE_DOWN);
	m_inSystem.setActionTrigger(AC_LEFT, SDL_SCANCODE_LEFT);
	m_inSystem.setActionTrigger(AC_RIGHT, SDL_SCANCODE_RIGHT);
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

void Scene::addSkyBody(double x, double y, double mass, std::string id, Uint8 r, Uint8 g, Uint8 b){

	std::string file = "ressources/skyBody.png";
	if (id == "player"){
		file = "ressources/test.png";
	}
	Sprite skyBodySprite(std::make_shared<SpriteSheet>(m_graSystem.loadTexture(file, r, g, b), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 }));

	if (id == "player"){
		skyBodySprite.addSpriteSheet("collision", std::make_shared<SpriteSheet>(m_graSystem.loadTexture("ressources/test_coll.png", r, g, b), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 }));
	}

	m_posComps.push_back(std::make_shared<PositionComponent>());
	
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps.back())); // on ajoute le dernier
	m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>(skyBodySprite)));

	m_gameObjects.push_back(GameObject(id));
	/*m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>(std::make_shared<SpriteSheet>(m_graSystem.loadTexture("ressources/test.png"), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 }))));
	//m_inputsComps.push_back(std::make_shared<InputsComponent>(std::make_shared<InputsSystem>(m_inSystem),std::make_shared<PhysicsComponent>(m_physicsComps.back())));
	//pas capable d'ajouter des components ;(
	m_gameObjects.push_back(GameObject());*/
	m_gameObjects.back().addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps.back());
	m_gameObjects.back().addComponent(std::type_index(typeid(GraphicsComponent)), m_graphicsComps.back());
	//m_gameObjects.back().addComponent(std::type_index(typeid(InputsComponent)), m_inputsComps.back());

	m_gameObjects.back().get<PhysicsComponent>()->setPosition(Vec2(x, y));
	m_gameObjects.back().get<PhysicsComponent>()->setMass(mass);
	//m_gameObjects.back().get<GraphicsComponent>()->setSprite(Sprite(SpriteSheet(m_graSystem.loadTexture("test.png"), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 })));
	std::cout << "added skyBody\n";
}

Scene::~Scene()
{

}

void Scene::update(Message &postman)
{
	m_inSystem.pollInputs();
	postman.clearAll();
	//std::cout << m_inSystem.isJoyConnected() << "\n";
	
	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Scene", "Input", MS_EXIT_REQUEST, 1);

	m_graSystem.initFrame();
	m_graSystem.setCameraZoom(0.3);
	
	for (int i = 0; i < m_gameObjects.size(); i++){

		//on update chaque component de l'objet
		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i].get<PhysicsComponent>();
		if (pc != nullptr){

			if (m_gameObjects[i].getID() == "player_copy") {
			//on check la direction du joueur
				Vec2 forces(0, 0);
				if (m_inSystem.checkTriggeredAction(AC_UP))
					forces += Vec2(0, -440);

				if (m_inSystem.checkTriggeredAction(AC_DOWN))
					forces += Vec2(0, 440);

				if (m_inSystem.checkTriggeredAction(AC_LEFT))
					forces += Vec2(-440, 0);

				if (m_inSystem.checkTriggeredAction(AC_RIGHT))
					forces += Vec2(440, 0);

				pc->setForces(forces);

				double speed = pc->getVelocity().getLength();
				
				//std::cout << m_graSystem.getZoom()<<  "  /  " << speed << "\n";

				if (m_graSystem.getZoom() >= 0.05  && m_graSystem.getZoom() <= 0.3) {
					m_graSystem.setCameraZoom((25000 / (speed + 1)) / 10);
					if (m_graSystem.getZoom() < 0.05)
						m_graSystem.setCameraZoom(0.15);
					if (m_graSystem.getZoom() > 0.3)
						m_graSystem.setCameraZoom(0.3);
				}
			}
			
			m_phySystem.update(postman, *pc, m_physicsComps, 1.0 / m_graSystem.getFPS());
		}

		auto gc = m_gameObjects[i].get<GraphicsComponent>();
		if (gc != nullptr){

			if (m_gameObjects[i].getID() == "player_copy"){
				m_graSystem.setCameraTarget(m_gameObjects[i].get<PhysicsComponent>()->getPosition());
			}

			//m_graSystem.lockCamera(false);
			m_graSystem.update(postman, *gc, 1.0 / m_graSystem.getFPS());
		}
	}
	//std::cout << m_graSystem.getFPS() << "\n";
	m_graSystem.endFrame();
}
