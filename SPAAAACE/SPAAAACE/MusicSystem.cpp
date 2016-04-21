#include "MusicSystem.h"
#include <iostream>

using namespace std;

MusicSystem::MusicSystem()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	
	init();
	//m_musique["menu"].play(10000); //TODO: le gérer dans update()!!
}

MusicSystem::~MusicSystem()
{
	Mix_CloseAudio();
}

void MusicSystem::update(Message &postman)
{
	if (postman.getMessage("main", "main", MS_SWITCHED))
		m_musique["menu"].play(10000);
	if (postman.getMessage("player", "player", MS_ENGINE_ACTIVE))
	{
		m_son["thrust"].play(150, 0, -1);
	}
	else 
		m_son["thrust"].stop();
	if (postman.getMessage("game", "Input", MS_SHOOT))
	{
		m_son["shoot"].play();
	}
	
}

void MusicSystem::init()
{
	//SON
	m_son["shoot"] = *(new Son("ressources/Son/shoot.wav",0));
	m_son["thrust"] = *(new Son("ressources/Son/thrust.wav",1));
	//MUSIQUE
	m_musique["zone1"] = *(new Musique("ressources/Musique/zone1.wav"));
	m_musique["menu"] = *(new Musique("ressources/Musique/menu.wav"));
}