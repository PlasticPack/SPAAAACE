#include "Scene.h"


Scene::Scene()
{
	init();
}


void Scene::init(){
	//ajout des étoiles en background
	srand(time(NULL));
	for (int i = 0; i < 5; i++){
		addSkyBody(rand() % 10000 - 5000, rand() % 10000 - 5000, rand() % 100 + 150, "shootingStar_" + std::to_string(m_gameObjects.size()), rand() % 100 + 150, rand() % 100 + 150);
		m_gameObjects.back().get<PhysicsComponent>()->activate(false);
		m_gameObjects.back().get<PhysicsComponent>()->setVelocity(Vec2(rand() % 1200 - 765, rand() % 400 - 205));
		m_gameObjects.back().get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.09);
	}

	addSkyBody(180, 110, 1, "skyBody_" + std::to_string(m_gameObjects.size()), 25, 240, 25);

	m_gameObjects.back().get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.3);

	addSkyBody(140, 100, 160, "skyBody_" + std::to_string(m_gameObjects.size()), 255, 100, 100);
	//m_gameObjects.back().get<PhysicsComponent>()->setHitboxRadius(90);
	//m_gameObjects.back().get<GraphicsComponent>()->setSize(Vec2(180, 180));

	addSkyBody(230, 450, 8, "player");
	m_gameObjects.back().get<PhysicsComponent>()->setVelocity(Vec2(15,3));

	std::cout << "END OF INIT\n\n";

	m_gameObjects.back().get<GraphicsComponent>()->setHaloColor({ 150, 200, 255, 255 });

	orderByZIndex();
	m_graSystem.loadBackground("ressources/space_1.png", 0, 150, 100, 100);
	m_graSystem.loadBackground("ressources/space_2.png", 1, 150, 100, 150);
	m_graSystem.loadBackground("ressources/space_3.png", 2, 200, 220, 255);
	//m_graSystem.loadBackground("ressources/bg1.png", 3);

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

	Sprite skyBodySprite(std::make_shared<SpriteSheet>(m_graSystem.loadTexture("ressources/test.png", r, g, b), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 }));

	m_posComps.push_back(std::make_shared<PositionComponent>());
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps.back())); // on ajoute le dernier
	m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>(skyBodySprite)));

	m_gameObjects.push_back(GameObject(id));
	m_gameObjects.back().addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps.back());
	m_gameObjects.back().addComponent(std::type_index(typeid(GraphicsComponent)), m_graphicsComps.back());
	
	m_gameObjects.back().get<PhysicsComponent>()->setPosition(Vec2(x, y));
	m_gameObjects.back().get<PhysicsComponent>()->setMass(mass);
	//m_gameObjects.back().get<GraphicsComponent>()->setSprite(Sprite(SpriteSheet(m_graSystem.loadTexture("test.png"), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 })));
	std::cout << "added skyBody\n";
}

Scene::~Scene()
{

}

void Scene::update()
{
	m_graSystem.initFrame();
	m_graSystem.setCameraZoom(1.2);
	for (int i = 0; i < m_gameObjects.size(); i++){

		//on update chaque component de l'objet
		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i].get<PhysicsComponent>();
		if (pc != nullptr){
			m_phySystem.update(*pc, m_physicsComps, 1.0 / 60.0);
		}

		auto gc = m_gameObjects[i].get<GraphicsComponent>();
		if (gc != nullptr){

			if (m_gameObjects[i].getID() == "player_copy"){
				m_graSystem.setCameraTarget(m_gameObjects[i].get<PhysicsComponent>()->getPosition() + (gc->getSize() / 2));
			}

			//m_graSystem.lockCamera(false);
			m_graSystem.update(*gc, 1.0 / 60.0);
		}
	}
	m_graSystem.endFrame();
}
