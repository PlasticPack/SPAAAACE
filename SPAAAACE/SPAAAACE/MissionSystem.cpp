#include "MissionSystem.h"
#include <LuaBridge.h>

std::map<std::string, s_objective> MissionSystem::m_objectives;
std::string MissionSystem::m_currentObjective = "objective_1";
Vec2 MissionSystem::m_objPosition = Vec2(0, 0);

MissionSystem::MissionSystem()
{
}


MissionSystem::~MissionSystem()
{
}

void MissionSystem::addObjective(luabridge::LuaRef& componentTable){

	using namespace luabridge;
	auto name = componentTable["name"];
	auto target = componentTable["target"];
	
	if (name.isString()){
		m_objectives["objective_" + std::to_string(m_objectives.size() + 1)] = { name, "none", false };
		if (target.isString()){
			m_objectives["objective_" + std::to_string(m_objectives.size())].targetID = target.cast<std::string>();
		}
		m_currentObjective = m_objectives.begin()->first;
	}
}

void MissionSystem::init(){
	
	//m_objectives["Profit"] = false;
	//m_objectives["Roger"] = false;
	//m_currentObjective = m_objectives.begin()->second.first;
}

void MissionSystem::update(Message &postman, std::map<std::string, std::shared_ptr<GameObject>> &objs){
	//check si message correspond à objectif updaté
	//et update objectif selon msg

	/*for (auto i = m_objectives.begin(); i != m_objectives.end(); i++){
		if (postman.getMessage("Scene", i->first, MS_OBJECTIVE) != 0){
			i->second = true;
		}
	}*/

	if (m_currentObjective != "null") {
		//std::cout << m_objectives[m_currentObjective].targetID << "\n";

		//if (m_objectives[m_currentObjective].targetID  == "yog_sogoth"){
			//std::cout << "??";
		//}

		if (objs.find(m_objectives[m_currentObjective].targetID) != objs.end()){
			if (m_objectives[m_currentObjective].targetID != "none"){

				//std::cout << m_objectives[m_currentObjective].targetID  << "\n";

				m_objPosition = objs[m_objectives[m_currentObjective].targetID]->get<PositionComponent>()->getPosition();

				objs[m_currentObjective]->get<PositionComponent>()->setPosition(m_objPosition);
			}
			else {
				m_objPosition = objs[m_currentObjective]->get<PositionComponent>()->getPosition();
			}
		}
		else if (objs.find(m_currentObjective) != objs.end()){
			m_objPosition = objs[m_currentObjective]->get<PositionComponent>()->getPosition();
		}
	}

	if (postman.getMessage("Action", m_currentObjective, MS_OBJECTIVE) == true){
		//std::cout << "JEUEJEJKAGDPIUPOSHDUIOAHSDSDAG:I\n\nHGDPIASDUIASDH\n\n";
		m_objectives[m_currentObjective].done = true;
		//std::cout << m_currentObjective << "\n";
		std::map < std::string, s_objective >::iterator it = m_objectives.find(m_currentObjective);
		it++;
		//std::cout << it->first << "\n";
		if (it != m_objectives.end()){
			std::cout << "NEXT OBJ\n";
			m_currentObjective = it->first;
		}
		else {
			m_currentObjective = "null";
			postman.addMessage("MissionSystem", "Mission", MS_MISSION_OVER, 1);
		}
		//passe au prochain objectif
	}

	GraphicsSystem::printAt("OBJECTIF", 25, 170);
	GraphicsSystem::printAt("_____________", 25, 180);
	GraphicsSystem::printAt(m_objectives[m_currentObjective].name, 25, 215);
}

Vec2 getPointerPosition(Vec2 direction, int half_size){

	Vec2 screen = Vec2(SCREEN_W, SCREEN_H);
	double l = 1;

	//double size = 25000.0 / (direction.getLength());

	/*if (size > 1)
		size = 1;
	if (size < 0.5)
		size = 0.5;*/

	if (abs(direction.getAngle()) < screen.getAngle() || direction.getAngle() > -180 - screen.getAngle()){
		l = sqrt(pow(SCREEN_W / 2, 2) + pow(tan(direction.getAngle() * (3.14159 / 180.0)) * SCREEN_W / 2, 2));
	}

	if (abs(direction.getAngle()) > screen.getAngle() && abs(direction.getAngle()) < 180 - screen.getAngle()){
		l = sqrt(pow(SCREEN_H / 2, 2) + pow(tan((90 + direction.getAngle()) * (3.14159 / 180.0)) * SCREEN_H / 2, 2));
	}

	l -= (half_size + 25);

	return Vec2((direction.getNormalized() * (l)) + Vec2(SCREEN_W / 2 - half_size, SCREEN_H / 2 - half_size));
}