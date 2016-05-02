#include "CinemaSystem.h"
#include "PhysicsComponent.h"
#include "Command.h"
#include <fstream>
using namespace std;

CinemaSystem::CinemaSystem() : m_isPlaying(false), m_isPhysics(true)
{

}
CinemaSystem::~CinemaSystem()
{

}

void CinemaSystem::updateObject(GameObject* objet, Message &postman)
{
	if (m_intro.isPlaying())
		m_intro.update(objet,postman);
	else m_isPlaying = false;
}

void CinemaSystem::update(Message &postman)
{
	if (postman.getMessage("game", "Input", MS_SHOOT))
	{
		m_intro.setPlaying(true);
		m_isPlaying = true;
	}
}

