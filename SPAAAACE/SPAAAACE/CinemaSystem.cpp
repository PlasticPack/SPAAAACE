#include "CinemaSystem.h"
#include "PhysicsComponent.h"
#include "Command.h"
#include <fstream>
using namespace std;

CinemaSystem::CinemaSystem() 
{

}
CinemaSystem::~CinemaSystem()
{

}

void CinemaSystem::updateObject(GameObject* objet, Message &postman)
{
	if (objet->getID() == "player")
	{
		m_cinematique["spin"].update(objet,postman);
		
		m_isPlaying = m_cinematique["spin"].isPlaying();
	}
}

void CinemaSystem::update(Message &postman)
{
	if (postman.getMessage("game", "Input", MS_SHOOT))
	{
		//m_isPlaying = true;
	}
}

