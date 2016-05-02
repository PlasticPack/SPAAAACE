#include "cinematique.h"
#include "PhysicsComponent.h"

using namespace std;


Cinematique::Cinematique() :m_isPlaying(false), m_isDone(false)
{

}
Cinematique::~Cinematique() 
{

}

bool Cinematique::update(GameObject* objet, Message &postman)
{
	return true;
}

CinematiqueIntro::CinematiqueIntro() : Cinematique(), m_stop(10)
{
	int dialogueNumber = 3;
	string* s = new string[dialogueNumber]{"\"Perdue\"","\"Tout seul\"","\"TOI:  Il ne faut pas perdre espoir...\""};
	m_text = *(new CommandText(s, dialogueNumber));
}
CinematiqueIntro::~CinematiqueIntro()
{

}
bool CinematiqueIntro::update(GameObject* objet, Message &postman) //TODO: controller la physique
{
	m_isPlaying = true;
	if (objet->getID() == "player")
	if (m_stop.isDone())
	{

		if (m_text.isDone())
		{
			m_isPlaying = false;
			m_isDone;
			return true;
		}
		else
			m_text.execute(objet);
	}
	else (m_stop.execute(objet));
	return m_isDone;
}

CinematiqueFirstBase::CinematiqueFirstBase() : Cinematique(), m_stop(1000)
{
	int dialogueNumber = 3;
	string* s = new string[dialogueNumber]{"", "\"Tout seul\"", "\"TOI:  Il ne faut pas perdre espoir...\""};
	m_text = *(new CommandText(s, dialogueNumber));
}
CinematiqueFirstBase::~CinematiqueFirstBase()
{

}
bool CinematiqueFirstBase::update(GameObject* objet, Message &postman)
{
	m_isPlaying = true;
	if (objet->getID() == "player")
		if (m_stop.isDone())
		{

		if (m_text.isDone())
		{
			m_isPlaying = false;
			m_isDone;
			return true;
		}
		else
			m_text.execute(objet);
		}
		else (m_stop.execute(objet));
		return m_isDone;
}