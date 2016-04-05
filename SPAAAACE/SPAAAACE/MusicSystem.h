#pragma once

#include <SDL_mixer.h>
#include "Son.h"
#include "Musique.h"
#include <map>
#include <string>
#include "Message.h"
class MusicSystem
{
private:
	std::map < std::string, Musique > m_musique;
	std::map < std::string, Son > m_son;
	void init();
public:
	//Constructeur/ Destructeur
	MusicSystem();
	~MusicSystem();

	//Fonctions
	void update(Message &postman); // Hard-code de music
};