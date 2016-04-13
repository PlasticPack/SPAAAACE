#include "MissionSystem.h"
#include <LuaBridge.h>

std::map<std::string, std::pair<std::string, bool>> MissionSystem::m_objectives;
std::string MissionSystem::m_currentObjective = "objective_1";

MissionSystem::MissionSystem()
{
}


MissionSystem::~MissionSystem()
{
}

void MissionSystem::addObjective(luabridge::LuaRef& componentTable){

	using namespace luabridge;
	auto name = componentTable["name"];
	
	if (name.isString()){
		m_objectives["objective_" + std::to_string(m_objectives.size() + 1)] = std::pair<std::string, bool>(name, false);
		m_currentObjective = m_objectives.begin()->first;
	}
}

void MissionSystem::init(){
	
	//m_objectives["Profit"] = false;
	//m_objectives["Roger"] = false;
	//m_currentObjective = m_objectives.begin()->second.first;
}

void MissionSystem::update(Message &postman){
	//check si message correspond à objectif updaté
	//et update objectif selon msg

	/*for (auto i = m_objectives.begin(); i != m_objectives.end(); i++){
		if (postman.getMessage("Scene", i->first, MS_OBJECTIVE) != 0){
			i->second = true;
		}
	}*/

	if (postman.getMessage("Action", m_currentObjective, MS_OBJECTIVE) == true){
		m_objectives[m_currentObjective].second = true;
		//std::cout << m_currentObjective << "\n";
		std::map < std::string, std::pair<std::string, bool> >::iterator it = m_objectives.find(m_currentObjective);
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
	GraphicsSystem::printAt(m_objectives[m_currentObjective].first, 25, 215);
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

	l -= half_size - 10;

	return Vec2((direction.getNormalized() * (l)) + Vec2(SCREEN_W / 2 - half_size, SCREEN_H / 2 - half_size));
}