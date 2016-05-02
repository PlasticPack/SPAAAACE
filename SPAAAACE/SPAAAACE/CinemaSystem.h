#pragma once
#include "Message.h"
#include "GameObject.h"
#include "cinematique.h"
#include <vector>
#include "Command.h"
class CinemaSystem
{
private:
	bool m_isPlaying;
	bool m_isPhysics;
	CinematiqueIntro m_intro;

public:
	CinemaSystem(); // TODO: prend les cinématique d'un fichier texte
	~CinemaSystem();

	void updateObject(GameObject* objet, Message &postman);
	void update(Message &postman);
	
	

	bool isPhysics() { return m_isPhysics; }
	bool isPlaying(void) { return m_isPlaying; }
	
	
};
