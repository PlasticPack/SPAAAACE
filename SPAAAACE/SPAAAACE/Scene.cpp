#include "Scene.h"


Scene::Scene(std::string scripts, std::string xml, std::string id)
{
	m_id = id;

	std::map<std::string, std::shared_ptr<GameObject>> source_objs;
	std::vector<std::shared_ptr<GameObject>> final_objs_vec;

	luain::loadFromRep(source_objs, scripts);
	XML_u::loadObjects(final_objs_vec, source_objs, xml);
	//transfo vector en map

	for (auto& it : final_objs_vec){

		m_gameObjects[it->getID()] = it;
		for (auto& s : it->getComponents()){
			std::cout << "\nAdding " << s.first.name() << " from " << it->getID() << " to " << id;

			if (s.first == std::type_index(typeid(PositionComponent))){
				m_posComps.push_back(std::dynamic_pointer_cast<PositionComponent>(s.second));
			}

			if (s.first == std::type_index(typeid(PhysicsComponent))){
				m_physicsComps.push_back(std::dynamic_pointer_cast<PhysicsComponent>(s.second));
			}

			if (s.first == std::type_index(typeid(GraphicsComponent))){
				m_graphicsComps.push_back(std::dynamic_pointer_cast<GraphicsComponent>(s.second));
			}

			if (s.first == std::type_index(typeid(GameLogicComponent))){
				m_GLComps.push_back(std::dynamic_pointer_cast<GameLogicComponent>(s.second));
			}

			if (s.first == std::type_index(typeid(ActionComponent))){
				m_ActionComps.push_back(std::dynamic_pointer_cast<ActionComponent>(s.second));
			}

			if (s.first == std::type_index(typeid(AiComponent))){
				m_AiComps.push_back(std::dynamic_pointer_cast<AiComponent>(s.second));
			}
		}
	}

	init();
}


void Scene::init(){

	GraphicsSystem::init();
	MissionSystem::init();
	//AJOUTEZ VOS OBJETS ICI! **********************************************


	try {
		//luain::loadFromRep(this, m_gameObjects, arg);
	}
	catch (std::exception e){
		std::cout << e.what() << "\n";

	}

	//À PARTIR DE CE POINT, N'AJOUTEZ PLUS D'OBJETS  ***********************


	///AJOUT BACKGROUND ET MACHINs

	orderByZIndex(); 

	if (m_id == "menu"){
		m_focusedID = "button_play";
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

	GraphicsSystem::setFont("ressources/CaviarDreams.ttf", 30, { 225, 220, 255 });

	std::cout << "END OF INIT\n\n";
	GraphicsSystem::setCameraZoom(1);
	m_navigationTimer.start();
	m_dialogueTimer.start();
}

void Scene::orderByZIndex(){
	if (m_gameObjects.size() > 1){

		std::vector < std::pair<std::string, double> > vec;
		for (auto const& it : m_gameObjects){
			if (it.second->get<PositionComponent>() != nullptr){
				vec.push_back(std::pair < std::string, double>(it.second->getID(), it.second->get<PositionComponent>()->getZIndex()));
			}
			else {
				vec.push_back(std::pair < std::string, double>(it.second->getID(), 0));
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

		
		/*std::vector< std::pair<std::string, std::shared_ptr<GameObject>>> vecGO(m_gameObjects.begin(), m_gameObjects.end());

		int j = 0;
		bool ordered = true;

		do {
			
			for (int i = 0; i < vecGO.size() - 1; i++){
				//std::cout << i << m_gameObjects[i]->getID() << " & " << m_gameObjects[i+1]->getID();
			
				if (vecGO[i].second->get<PositionComponent>()->getZIndex() > vecGO[i+1].second->get<PositionComponent>()->getZIndex()){
					std::cout << " just swapped\n";
					std::iter_swap(vecGO[i].second, vecGO[i +1].second);
					//swapped = true;
				}
				else 
					std::cout << "\n";

			}

			ordered = true;
			for (int i = 0; i < vecGO.size() - 1; i++){
				//std::cout << i << m_gameObjects[i]->getID() << " & " << m_gameObjects[i+1]->getID();

				if (vecGO[i].second->get<PositionComponent>()->getZIndex() > vecGO[i].second->get<PositionComponent>()->getZIndex()){
					ordered = false;
					break;
				}
			}

		} while (!ordered);

		//std::reverse(m_gameObjects.begin(), m_gameObjects.end());

		//on place le joueur au "dessus" des objets à sa même hauteur
		for (int i = 0; i < vecGO.size(); i++){
			if (vecGO[i].second->getID() == "player"){
				if (i != vecGO.size() - 1) {
					for (int j = i; j < vecGO.size(); j++){
						if (vecGO[j].second->getID() != "player" && vecGO[j].second->get<PositionComponent>()->getZIndex() == vecGO[i].second->get<PositionComponent>()->getZIndex()) {
							std::iter_swap(vecGO.begin() + i, vecGO.begin() + j);
							std::cout << i << j << "\n";
							break;
						}
					}
				}
			}
		}

		//recopie du vector dans la map
		m_gameObjects.clear();
		for (auto const& k : vecGO){
			m_gameObjects[k.first] = k.second;
		}*/
	}
}

/*
bool zSort(std::shared_ptr<GameObject> g1, std::shared_ptr<GameObject> g2){

if (&g1 != &g2){

if (g1->hasComponent(std::type_index(typeid(PositionComponent))) && g2->hasComponent(std::type_index(typeid(PositionComponent)))) {
return (g1->get<PositionComponent>() > g2->get<PositionComponent>());
}

}
return false;
}

void Scene::orderByZIndex(){
/*if (m_gameObjects.size() > 0){
int j = 0;
bool swapped = false;
do {
for (int i(1); i < m_gameObjects.size(); i++){

if (m_gameObjects[i-1]->get<PositionComponent>()->getZIndex() < m_gameObjects[i]->get<PositionComponent>()->getZIndex()){
std::iter_swap(m_gameObjects.begin() + i -1, m_gameObjects.begin() + i);
swapped = true;
}

if (i == m_gameObjects.size() - 1 && !swapped){
swapped = true;
}
}
} while (!swapped);

//on place le joueur au "dessus" des objets à sa même hauteur
/*for (int i = 0; i < m_gameObjects.size(); i++){
if (m_gameObjects[i]->getID() == "player"){
if (i != m_gameObjects.size() - 1) {
for (int j = i; j < m_gameObjects.size(); j++){
if (m_gameObjects[j]->getID() != "player" && m_gameObjects[j]->get<PositionComponent>()->getZIndex() == m_gameObjects[i]->get<PositionComponent>()->getZIndex()) {
//std::iter_swap(m_gameObjects.begin() + i, m_gameObjects.begin() + j);
std::cout << i << j << "\n";
break;
}
}
}
}
}
}*

std::sort(m_gameObjects.begin(), m_gameObjects.end(), zSort);

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
//std::reverse(m_gameObjects.begin(), m_gameObjects.end());
}*/

Scene::~Scene()
{
	//GraphicsSystem::close();
}

void Scene::update(Message &postman)
{

	if (postman.getMessage("main", "main", MS_SWITCHED) == 1) {
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

	postman.clearAll();
	//INPUT GÉNÉRAL
	m_inSystem.pollInputs();
	postman.clearAll();
	if (m_inSystem.checkTriggeredAction(AC_START))
		postman.addMessage("game", "Input", 0, 1);

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
		
		if(currentObj->isActive()){
			if (m_orderedGO[i] == "planet_menu"){
				GraphicsSystem::setCameraTarget(currentObj->get<PositionComponent>()->getPosition() + Vec2(-SCREEN_W/2, 0));
			}

			std::shared_ptr<PhysicsComponent> pc = currentObj->get<PhysicsComponent>();
			if (pc != nullptr){
				/*if (currentID == MissionSystem::getCurrentObjective()){
					objectivePos = currentObj->get<PositionComponent>()->getPosition();
				}

				if (currentID == "base"){
					basePos = currentObj->get<PhysicsComponent>()->getPosition();
				}*/

				//DÉCISION DE MOUVEMENT : JOUEUR
				if (currentID == "player") {
					if (currentObj->get<GameLogicComponent>()->getCurrentFuel() > 0) { // si assez de fuel
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
				AiSystem::update(AiC, m_physicsComps, m_gameObjects["player"]->get<PhysicsComponent>());
			}

			auto gc = currentObj->get<GraphicsComponent>();
			if (gc != nullptr){

				Vec2 playerPos;
				Vec2 basePos;
				Vec2 objPos;
				if (m_id == "game"){
					playerPos = m_gameObjects["player"]->get<PhysicsComponent>()->getPosition();
					//basePos = m_gameObjects["base"]->get<PhysicsComponent>()->getPosition();
					objPos = MissionSystem::getObjPosition();
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
					if (currentID.find("hud") != std::string::npos) { // si l'objet fait partie du hud

						std::string id = currentID;

						if (id == "hud_fuel"){
							if (postman.getMessage("GameLogic", m_gameObjects["player"]->getID(), MS_ENGINE_ACTIVE) > 0){

								//on réduit le fuel
								Vec2 baseSize = currentObj->get<GraphicsComponent>()->getMaxSize();
								double fuel = (double)(m_gameObjects["player"]->get<GameLogicComponent>()->getCurrentFuel()) / (double)m_gameObjects["player"]->get<GameLogicComponent>()->getMaxFuel();
								currentObj->get<GraphicsComponent>()->setSize(Vec2(baseSize.x() * fuel, baseSize.y()));

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
							if (direction.getLength() > 0){
								currentObj->get<PositionComponent>()->setAngle(direction.getAngle());
								double size = 25000.0 / (direction.getLength());

								if (size > 1)
									size = 1;
								if (size < 0.5)
									size = 0.5;

								currentObj->get<GraphicsComponent>()->setSize(currentObj->get<GraphicsComponent>()->getMaxSize() *size);
								currentObj->get<PositionComponent>()->setPosition(getPointerPosition(direction, 32));
							}
						}
						else if (id == "hud_obj_pointer"){
							if (MissionSystem::getCurrentObjective() != "null"){
								Vec2 direction = objPos - playerPos;
								if (direction.getLength() > 0){

									currentObj->get<PositionComponent>()->setAngle(direction.getAngle());

									double size = 25000.0 / (direction.getLength());

									if (size > 1)
										size = 1;
									if (size < 0.5)
										size = 0.5;

									currentObj->get<GraphicsComponent>()->setSize(currentObj->get<GraphicsComponent>()->getMaxSize() *size);
									currentObj->get<PositionComponent>()->setPosition(getPointerPosition(direction, 32));
								}
							}
							else {
								//on affiche pas le truc
							}
						}
					}
				}

				GraphicsSystem::update(postman, currentID,*gc, 1.0 / GraphicsSystem::getFPS());
			}
		}

		if (postman.getMessage("GraphicsSystem", currentID, MS_OBJ_DONE) == 1){
			currentObj->activate(false);
		}
	}
		
	
	for (int i = 0; i < m_orderedGO.size(); i++){
		auto AC = m_gameObjects[m_orderedGO[i]]->get<ActionComponent>();
		if (AC != nullptr){
			ActionSystem::update(postman, *AC);
		}
	}

	if (m_id == "game"){
		MissionSystem::update(postman, m_gameObjects);
		if (postman.getMessage("MissionSystem", "Mission", MS_MISSION_OVER) == 1){
			//std::cout << "OVER!!!";
			GraphicsSystem::print("Mission terminée.");
		}
	}

	if (postman.getMessage("Action", "Trigger", 34061) > 0) {
		GraphicsSystem::print("TRRRIIIIGGGERREDDD oh thats rude");
	
	}
	
	//MusSystem
	m_musSytem.update(postman);

	if (m_inSystem.checkTriggeredAction(AC_EXIT))
		postman.addMessage("Action", "Button", MS_EXIT_REQUEST, 1);

	GraphicsSystem::endFrame(postman);

}
