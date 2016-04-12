#include "MissionSystem.h"

std::map<std::string, bool> MissionSystem::m_objectives;
std::string MissionSystem::m_currentObjective = "Aucun objectif à date";

MissionSystem::MissionSystem()
{
}


MissionSystem::~MissionSystem()
{
}

void MissionSystem::init(){
	
	m_objectives["Exploser le machin"] = false;
	m_objectives["Faire preuve de sensualité envers Guy-Dune Albator <3"] = false;
	m_objectives["????"] = false;
	m_objectives["Profit"] = false;
	m_objectives["Roger"] = false;
	m_currentObjective = "Exploser le machin";
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
		m_objectives[m_currentObjective] = true;
		std::map<std::string, bool>::iterator it = m_objectives.find(m_currentObjective);
		it++;
		if (it != m_objectives.end()){
			m_currentObjective = it->first;
		}
		//passe au prochain objectif
	}

	GraphicsSystem::printAt("OBJECTIF", 25, 170);
	GraphicsSystem::printAt("_____________", 25, 180);
	GraphicsSystem::printAt(m_currentObjective, 25, 215);
}