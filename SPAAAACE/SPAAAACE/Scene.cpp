#include "Scene.h"


Scene::Scene()
{
	init();
}


void Scene::init(){

	/*m_posComps.push_back(std::make_shared<PositionComponent>());
	m_posComps.push_back(std::make_shared<PositionComponent>());
	m_posComps.push_back(std::make_shared<PositionComponent>());
	m_posComps.push_back(std::make_shared<PositionComponent>());

	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps[0]));
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps[1]));
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps[2]));
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps[3]));
	
	GameObject player;
	player.addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps[0]);

	GameObject player2;
	player2.addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps[1]);

	GameObject player3;
	player3.addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps[2]);

	GameObject player4;
	player4.addComponent(std::type_index(typeid(PhysicsComponent)), m_physicsComps[3]);

	m_gameObjects.push_back(player);
	m_gameObjects.push_back(player2);
	m_gameObjects.push_back(player3);
	m_gameObjects.push_back(player4);

	m_gameObjects[0].get<PhysicsComponent>()->setPosition(Vec2(400, 180));
	m_gameObjects[0].get<PhysicsComponent>()->setMass(6.0);
	//m_gameObjects[0].get<PhysicsComponent>()->getPositionComponent()->setZIndex(0.01);
	m_gameObjects[1].get<PhysicsComponent>()->setPosition(Vec2(600, 200));
	//m_gameObjects[1].get<PhysicsComponent>()->setVelocity(Vec2(-1, 0));

	//std::cout << m_physicsComps[1]->getVelocity().getLength() << "\n";

	m_gameObjects[2].get<PhysicsComponent>()->setPosition(Vec2(100, 300));

	m_gameObjects[3].get<PhysicsComponent>()->setPosition(Vec2(200, 400));*/

	addSkyBody(810, 110, 1);
	addSkyBody(440, 300, 46);
	addSkyBody(230, 450, 2);
	//(340, 640, 8);

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	std::cout << "END OF INIT\n\n";

}

void Scene::addSkyBody(double x, double y, double mass){

	m_posComps.push_back(std::make_shared<PositionComponent>());
	
	m_physicsComps.push_back(std::make_shared<PhysicsComponent>(m_posComps.back())); // on ajoute le dernier
	m_graphicsComps.push_back(std::make_shared<GraphicsComponent>(m_posComps.back(), std::make_shared<Sprite>(std::make_shared<SpriteSheet>(m_graSystem.loadTexture("ressources/test.png"), SDL_Rect{ 0, 0, 30, 30 }, SDL_Rect{ 0, 0, 60, 60 }))));
	//m_inputsComps.push_back(std::make_shared<InputsComponent>(std::make_shared<InputsSystem>(m_inSystem),std::make_shared<PhysicsComponent>(m_physicsComps.back())));
	//pas capable d'ajouter des components ;(
	m_gameObjects.push_back(GameObject());
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
	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Scene", "Input", MS_EXIT_REQUEST, 1);

	m_graSystem.initFrame();
	for (int i = 0; i < m_gameObjects.size(); i++){

		//on update chaque component de l'objet
		std::shared_ptr<PhysicsComponent> pc = m_gameObjects[i].get<PhysicsComponent>();
		if (pc != nullptr){
			m_phySystem.update(*pc, m_physicsComps, 1.0 / 120.0);
		}

		auto gc = m_gameObjects[i].get<GraphicsComponent>();
		//std::cout << "ADRESS OF TEXTURE IN SCENEU : " << gc->getSprite()->getCurrentSpriteSheet().getTexture() << "\n";
		if (gc != nullptr){
			//m_graSystem.initFrame();
			m_graSystem.update(*gc, 1.0 / 120.0);
			//m_graSystem.endFrame();
		}

		//std::cout << "-------------\n";
	}
	m_graSystem.endFrame();
	//std::cout << "**************\n";
}
