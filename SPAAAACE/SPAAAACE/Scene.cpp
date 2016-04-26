#include "Scene.h"
#include "Command.h"

Scene::Scene(std::string arg, std::string xml, std::string id) : m_id(id), m_missionSystem(std::make_shared<MissionSystem>())
{
	init(arg, xml);
}


void Scene::init(std::string arg, std::string xml){

	GraphicsSystem::init();
	m_missionSystem->init();
	//AJOUTEZ VOS OBJETS ICI! **********************************************

	try {
		luain::loadFromRep(m_gameObjects,this, arg);
	}
	catch (std::exception e){
		std::cout << e.what() << "\n";
	}

	std::vector<std::shared_ptr<GameObject>> pure;
	XML_u::loadObjects(pure, m_gameObjects, xml);

	//modif des ids
	m_gameObjects.clear();
	std::map<std::string, int> mapID;
	std::string lastID = pure[0]->getID();
	
	int total = 1;
	for (int i = 1; i < pure.size(); i++){

		//si on a pas déjà setté l'ID
		if (!pure[i]->idSet()) {
			if (lastID != pure[i]->getID()){
				mapID[lastID] = total;
				total = 1;
			}
			else {
				total++;
			}

			if (i == pure.size() - 1){
				mapID[pure[i]->getID()] = total;
			}

			lastID = pure[i]->getID();
		}
		else {
			std::cout << ":(:(:(\n";
			mapID[pure[i]->getID()] = 1;
		}
	}

	for (int i = 0; i < pure.size(); i++){

		//création de l'ID de l'objet selon son ID précédent
		//ou simplement le i
		//sauf si player, là on laisse player
		std::string newID = pure[i]->getID();

		//si y'en a juste un dans le script, on mets pas de "1"
		//std::cout << "HEYHEY HEY" << newID << mapID[newID] << "\n";

		if (mapID[newID] != 1){
			//std::cout << "HEYHEY HEY" << newID <<  mapID[newID] << "\n";
			newID += std::to_string(i);
		}

		//std::cout << newID << "\n";
		m_gameObjects[newID] = pure[i];
		m_gameObjects[newID]->setID(newID);
		m_gameObjects[newID]->setType(pure[i]->getType());

		//on ajoute les components à la scene

		for (auto const& c : m_gameObjects[newID]->getComponents()){

			if (c.first == std::type_index(typeid(PositionComponent))){
				m_posComps.push_back(std::dynamic_pointer_cast<PositionComponent>(c.second));
			}

			if (c.first == std::type_index(typeid(PhysicsComponent))){
				m_physicsComps.push_back(std::dynamic_pointer_cast<PhysicsComponent>(c.second));
			}

			if (c.first == std::type_index(typeid(GraphicsComponent))){
				m_graphicsComps.push_back(std::dynamic_pointer_cast<GraphicsComponent>(c.second));
			}

			if (c.first == std::type_index(typeid(GameLogicComponent))){
				m_GLComps.push_back(std::dynamic_pointer_cast<GameLogicComponent>(c.second));
			}

			if (c.first == std::type_index(typeid(ActionComponent))){
				m_ActionComps.push_back(std::dynamic_pointer_cast<ActionComponent>(c.second));
			}

			if (c.first == std::type_index(typeid(AiComponent))){
				m_AiComps.push_back(std::dynamic_pointer_cast<AiComponent>(c.second));
			}

		}
	}

	//std::cout << "PHYSCADOCHAU : " << m_physicsComps.size() << "\n";
	//std::cout << "!!!!!\n\n\n\n";
	//std::cout << m_gameObjects["ai1"]->get<PositionComponent>()->getPosition().x();

	//À PARTIR DE CE POINT, N'AJOUTEZ PLUS D'OBJETS  ***********************
	
	///AJOUT BACKGROUND ET MACHINs

	orderByZIndex(); 

	if (m_id == "menu"){
		m_focusedID = "button_play";
	}
	else if (m_id == "savesMenu"){
		m_focusedID = "button_save1";
	}

	//Inputs init
	m_inSystem.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	
	//initialise le mouvement du joueur selon le clavier
	m_inSystem.setActionTrigger(AC_SHOOT, SDL_SCANCODE_SPACE);
	m_inSystem.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	m_inSystem.setActionTrigger(AC_UP, GP_DPAD_UP);
	m_inSystem.setActionTrigger(AC_DOWN, GP_DPAD_DOWN);
	m_inSystem.setActionTrigger(AC_START, SDL_SCANCODE_C);
	m_inSystem.setActionTrigger(AC_SELECT, SDL_SCANCODE_D);
	m_inSystem.setActionTrigger(AC_DOWN, SDL_SCANCODE_DOWN);
	m_inSystem.setActionTrigger(AC_LEFT, SDL_SCANCODE_LEFT);
	m_inSystem.setActionTrigger(AC_RIGHT, SDL_SCANCODE_RIGHT);
	m_inSystem.setActionTrigger(AC_HORIZONTAL_PUSH, GP_AXIS_LEFT_JOY_X);
	m_inSystem.setActionTrigger(AC_VERTICAL_PUSH, GP_AXIS_LEFT_JOY_Y);
	m_inSystem.setActionTrigger(AC_SELECT, SDL_SCANCODE_RETURN);
	m_inSystem.setActionTrigger(AC_SELECT, GP_BUTTON_A);

	m_inSystem.setActionTrigger(AC_NEXT, SDL_SCANCODE_N);
	m_inSystem.setActionTrigger(AC_PAUSE, SDL_SCANCODE_LCTRL);
	m_inSystem.setActionTrigger(AC_MAP, SDL_SCANCODE_M);

	GraphicsSystem::setFont("ressources/CaviarDreams.ttf", 30, { 225, 220, 255 });

	std::cout << "END OF INIT\n\n";
	GraphicsSystem::setCameraZoom(1);
	m_navigationTimer.start();
	m_dialogueTimer.start();
	m_selectTimer.start();
	m_pauseTimer.start();
	m_mapTimer.start();

	XML_u::saveObjects(m_gameObjects, "saves/final2.xml");
}

void Scene::orderByZIndex(){

	if (m_gameObjects.size() > 1){

		std::vector < std::pair<std::string, double> > vec;
		for (auto& it : m_gameObjects){

			if (it.second->get<PositionComponent>() != nullptr){
				vec.push_back(std::pair < std::string, double>(it.first, it.second->get<PositionComponent>()->getZIndex()));
			}
			else {
				vec.push_back(std::pair < std::string, double>(it.first, 0));
			}

		}

		bool ordered = true;
		do {

			for (int i = 0; i < vec.size() - 1; i++){
				if (vec[i].second > vec[i + 1].second){
					std::iter_swap(vec.begin() + i, vec.begin() + i +1);
				}
				else
					std::cout << "\n";
			}

			ordered = true;
			for (int i = 0; i < vec.size() - 1; i++){

				if (vec[i].second > vec[i +1].second){
					ordered = false;
					break;
				}
			}

		} while (!ordered);

		//on place le joueur au "dessus" des objets à sa même hauteur
		for (int i = 0; i < vec.size(); i++){
			if (vec[i].first == "player"){
				if (i != vec.size() - 1) {
					for (int j = i; j < vec.size(); j++){
						if (vec[j].first != "player" && vec[j].second == vec[i].second) {
							std::iter_swap(vec.begin() + i, vec.begin() + j);
							break;
						}
					}
				}
			}
		}

		for (int i = 0; i < vec.size();i++){
			m_orderedGO.push_back(vec[i].first);
			std::cout << vec[i].first << " " << vec[i].second << "\n";
		}
	}
}

Scene::~Scene()
{

	//GraphicsSystem::close();
}

void Scene::update(Message &postman)
{

	if (postman.getMessage("main", "main", MS_SWITCHED) == 1) {
		std::cout << "SWITCHED\n";
		if (m_id == "game"){ // pour le moment c'est ca
			GraphicsSystem::clearBackgrounds();
			GraphicsSystem::loadBackground("ressources/space_1.png", 0);
			GraphicsSystem::loadBackground("ressources/space_2.png", 1);
			GraphicsSystem::loadBackground("ressources/space_3.png", 2);
		}

		if (m_id == "menu"){ // pour le moment c'est ca
			GraphicsSystem::clearBackgrounds();
			GraphicsSystem::loadBackground("ressources/space_1.png", 0, 255, 100, 100);
			GraphicsSystem::loadBackground("ressources/space_2.png", 1, 100, 255, 100);
			GraphicsSystem::loadBackground("ressources/space_3.png", 2, 100, 100, 255);
		}
	}

	//INPUT GÉNÉRAL
	m_inSystem.pollInputs();
	postman.clearAll();
	if (m_inSystem.checkTriggeredAction(AC_START))
		postman.addMessage("game", "Input", 0, 1);

	if (m_inSystem.checkTriggeredAction(AC_PAUSE)) {
		if (m_pauseTimer.getTicks() > 450) {
			postman.addMessage("Scene", "Input", MS_PAUSE, 1);
			m_pauseTimer.stop();
			m_pauseTimer.start();
			m_pause = !m_pause;
		}
	}

	if (m_inSystem.checkTriggeredAction(AC_MAP)) {
		if (m_mapTimer.getTicks() > 375) {
			m_mapTimer.stop();
			m_mapTimer.start();
			m_map = !m_map;
		}
	}

	if (m_map){
		postman.addMessage("Scene", "Input", MS_MAP, 1);
	}



	//GESTION DU MENU

	//NAVIGATION AVEC FLÈCHES
	if (m_id == "menu" || m_id == "savesMenu"){

		//en attendant gestion press/release
		if (m_navigationTimer.getTicks() > 200){

			//création du "template" de menu
			std::vector<std::string> menuList;
			if (m_id == "menu") {
				menuList.push_back("button_play");
				menuList.push_back("button_quit");
			}
			else {
				menuList.push_back("button_save1");
				menuList.push_back("button_save2");
				menuList.push_back("button_save3");
			}

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
		if (m_selectTimer.getTicks() > 370){
			std::cout << "HEY";
			postman.addMessage("Scene", m_focusedID, MS_SELECT, 1);
			m_selectTimer.stop();
			m_selectTimer.start();
		}
	}

	if (m_inSystem.checkTriggeredAction(AC_NEXT)) {
		if (m_dialogueTimer.getTicks() > 575) {
			postman.addMessage("Scene", "Input", MS_DIALOGUE_NEXT, 1); 
			m_dialogueTimer.stop();
			m_dialogueTimer.start();
		}
	}
	
	//TIR DU VAISSEAU
	if (m_inSystem.checkTriggeredAction(AC_SHOOT))
	{
		//std::cout << "pew!\n";
		postman.addMessage("game", "Input", MS_SHOOT, 1);
		
		
	}

	GraphicsSystem::initFrame();


	for (int i = 0; i < m_orderedGO.size(); i++){

		std::string currentID = m_orderedGO[i];
		auto currentObj = m_gameObjects[m_orderedGO[i]];

		if (currentObj->isActive()){

			if (m_orderedGO[i].find("planet_menu") != std::string::npos){
				GraphicsSystem::setCameraTarget(currentObj->get<PositionComponent>()->getPosition() + Vec2(-SCREEN_W / 2, 0));
			}

			std::shared_ptr<PhysicsComponent> pc = currentObj->get<PhysicsComponent>();
			if (pc != nullptr && !m_pause && !m_map){
				//DÉCISION DE MOUVEMENT : JOUEUR
				if (!m_cineSystem.isPlaying()){
					if (currentID == "player") {
						if (currentObj->get<GameLogicComponent>()->getCurrentFuel() > 0 && currentObj->get<GameLogicComponent>()->getCurrentLife() > 0) { // si assez de fuel
							//on check la direction du joueur
							Vec2 forces(0, 0);
							Vec2 direction(cos(pc->getPositionComponent()->getAngle() * (3.14159 / 180.0)), sin(pc->getPositionComponent()->getAngle() * (3.14159 / 180.0)));
							int pwr = currentObj->get<GameLogicComponent>()->getEnginePower();

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
								postman.addMessage("player", "player", MS_ENGINE_ACTIVE, 1);

								//on baisse le fuel

								postman.addMessage("Scene", currentID, MS_ENGINE_ACTIVE, 1);
								pc->getPositionComponent()->setAngle(angle);

								pc->setForces(forces);
							}
						}
					}
					else
					{
						m_cineSystem.updateObject(currentObj.get(), postman);
					}
				}
				PhysicsSystem::update(postman, this, *pc, m_physicsComps, 1.0 / GraphicsSystem::getFPS());
			}

			if (postman.getMessage("Physics", currentID, MS_COLLISION) > 1){
				postman.addMessage(currentID, getFatherID<PhysicsComponent>(postman.getMessage("Physics", currentID, MS_COLLISION)), MS_COLLISION, 1);
			}

			auto GLC = currentObj->get<GameLogicComponent>();
			if (GLC != nullptr){
				GameLogicSystem::update(postman, currentObj, *GLC, 1.0 / GraphicsSystem::getFPS());
			}

			auto AiC = currentObj->get<AiComponent>();
			if (AiC != nullptr){
				//std::cout << "AIAIAIAIA";
				AiSystem::update(AiC, m_physicsComps, m_gameObjects["player"]->get<PhysicsComponent>());
			}

			auto gc = currentObj->get<GraphicsComponent>();
			if (gc != nullptr){

				bool updateGS = true;

				Vec2 playerPos;
				Vec2 basePos;
				Vec2 objPos;
				if (m_id == "game"){
					playerPos = m_gameObjects["player"]->get<PhysicsComponent>()->getPosition();
					basePos = m_gameObjects["base"]->get<PhysicsComponent>()->getPosition();
					objPos = m_missionSystem->getObjPosition();
				}

				if (currentID == m_focusedID){
					gc->getSprite()->setSpriteSheet("selected");
				}
				else {
					gc->getSprite()->setSpriteSheet("default");
				}

				if (currentID == "player"){

					double vel = currentObj->get<PhysicsComponent>()->getVelocity().getLength();
					double zoom = 0.685;

					GraphicsSystem::setCameraZoom(zoom);
					GraphicsSystem::setCameraTarget(gc->getPosition());

				}

				if (m_id == "game"){
					if (m_pause){
						GraphicsSystem::printAt("Pause", 500, 250, 400, 100);
					}

					if (currentID.find("hud") != std::string::npos) { // si l'objet fait partie du hud

						std::string id = currentID;

						if (id == "hud_fuel"){
							if (postman.getMessage("GameLogic", m_gameObjects["player"]->getID(), MS_ENGINE_ACTIVE) > 0){

								//on réduit le fuel
								Vec2 baseSize = currentObj->get<GraphicsComponent>()->getMaxSize();
								double fuel = (double)(m_gameObjects["player"]->get<GameLogicComponent>()->getCurrentFuel()) / (double)m_gameObjects["player"]->get<GameLogicComponent>()->getMaxFuel();
								currentObj->get<GraphicsComponent>()->setSize(Vec2(baseSize.x() * fuel, baseSize.y()));
								postman.addMessage("HUD", "FUEL", MS_FUEL_LEVEL, m_gameObjects["player"]->get<GameLogicComponent>()->getCurrentFuel());
							}
						}
						else if (id == "hud_life"){
							//if (postman.getMessage("GameLogic", currentID, MS_LIFE_DOWN) > 0) {

							Vec2 baseSize = currentObj->get<GraphicsComponent>()->getMaxSize();
							double life = (double)(m_gameObjects["player"]->get<GameLogicComponent>()->getCurrentLife()) / (double)m_gameObjects["player"]->get<GameLogicComponent>()->getMaxLife();
							currentObj->get<GraphicsComponent>()->setSize(Vec2(baseSize.x() * life, baseSize.y()));
							//}
						}
						else if (id == "hud_base_pointer"){
							Vec2 direction = basePos - playerPos;
							if (direction.getLength() > SCREEN_W){
								currentObj->get<PositionComponent>()->setAngle(direction.getAngle());
								double size = 25000.0 / (direction.getLength());

								if (size > 1)
									size = 1;
								if (size < 0.5)
									size = 0.5;

								currentObj->get<GraphicsComponent>()->setSize(currentObj->get<GraphicsComponent>()->getMaxSize() *size);
								currentObj->get<PositionComponent>()->setPosition(getPointerPosition(direction, 32));
							}
							else
								updateGS = false;
						}
						else if (id == "hud_obj_pointer"){
							if (m_missionSystem->getCurrentObjective() != "null"){
								Vec2 direction = objPos - playerPos;
								if (direction.getLength() > SCREEN_W){

									currentObj->get<PositionComponent>()->setAngle(direction.getAngle());

									double size = 25000.0 / (direction.getLength());

									if (size > 1)
										size = 1;
									if (size < 0.5)
										size = 0.5;

									currentObj->get<GraphicsComponent>()->setSize(currentObj->get<GraphicsComponent>()->getMaxSize() *size);
									currentObj->get<PositionComponent>()->setPosition(getPointerPosition(direction, 32));
								}
								else
									updateGS = false;
							}
							else {
								//on affiche pas le truc
								updateGS = false;
							}
						}
					}
				}
				if (updateGS && !m_map)
					GraphicsSystem::update(postman, currentID, *gc, 1.0 / GraphicsSystem::getFPS());
			}
		
		
			if (postman.getMessage("GraphicsSystem", currentID, MS_OBJ_DONE) == 1){
				currentObj->activate(false);
			}

		}
	}

	std::map<std::string, std::shared_ptr<GameObject>>::iterator it;
	for (auto& it : m_gameObjects){
		auto AC = it.second->get<ActionComponent>();
		if (AC != nullptr){
			ActionSystem::update(postman, *AC);
		}
	}

	if (m_id == "game"){
		m_missionSystem->update(postman, m_gameObjects);

		//std::cout << m_missionSystem->getCurrentObjective() << "\n";

		if (postman.getMessage("MissionSystem", "Mission", MS_MISSION_OVER) == 1){
			//std::cout << "OVER!!!";
			GraphicsSystem::print("Mission terminée.");
		}
	}

	if (postman.getMessage("Action", "Trigger", 34061) > 0) {
		GraphicsSystem::print("TRRRIIIIGGGERREDDD oh thats rude");
	
	}
	
	m_cineSystem.update(postman);
	
	//MusSystem
	m_musSytem.update(postman);

	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Action", "Button", MS_EXIT_REQUEST, 1);

	GraphicsSystem::endFrame(postman, m_gameObjects);
}
