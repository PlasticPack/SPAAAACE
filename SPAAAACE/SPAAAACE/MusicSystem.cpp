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
	//m_musique["menu"].play(100); //TODO: le gérer dans update()!!
}

MusicSystem::~MusicSystem()
{
	Mix_CloseAudio();
}

void MusicSystem::update(Message &postman)
{
	//MUSIQUE
	if (postman.getMessage("Scene", "ID", MS_MENU))
	{
		//if (!Mix_PlayingMusic())
		m_musique["menu"].play(5000);
	}
	else 
	{
		if (m_musique["menu"].isPlaying())
		{
			m_musique["menu"].stop();
		}
		else if (postman.getMessage("Scene", "Input", MS_PAUSE))
		{
			if (!m_musique["pause"].isPlaying())
			{
				m_musique["zone1"].stop();
				m_musique["pause"].play();

			}
				else
				{
					m_musique["pause"].stop();
				}
		}
		else if (!m_musique["pause"].isPlaying())
			m_musique["zone1"].play();
		
			
	}

	//SON
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
	
	//avertisseur de fuel
	double fuel = postman.getMessage("HUD", "FUEL", MS_FUEL_LEVEL);
	cout << fuel << endl;
	if (fuel != 0)
	{

		Mix_VolumeMusic(fuel * 512);
		if (fuel < 0.001)
		{
			m_son["fuelEmpty"].play(0, 0, 0);
			m_son["fuelLow1"].stop();
			m_son["fuelLow2"].stop();
		}
		else if (fuel < 0.075)
		{
			m_son["fuelLow2"].play(0, 0, -1);
			m_son["fuelLow1"].stop();
		}
		else if (fuel < 0.15)
		{
			m_son["fuelLow1"].play(0, 0, -1);

		}
		else
		{
			m_son["fuelLow1"].stop();
			m_son["fuelLow2"].stop();
		}
	}
	
}

void MusicSystem::init()
{
	//SON
	m_son["shoot"] = *(new Son("ressources/Son/shoot.wav",0));
	m_son["thrust"] = *(new Son("ressources/Son/thrust.wav",1));
	m_son["fuelLow1"] = *(new Son("ressources/Son/fuelLow1.wav", 2));
	m_son["fuelLow2"] = *(new Son("ressources/Son/fuelLow2.wav", 3));
	m_son["fuelEmpty"] = *(new Son("ressources/Son/fuelEmpty.wav", 4));
	//MUSIQUE
	m_musique["zone1"] = *(new Musique("ressources/Musique/zone1.wav"));
	m_musique["menu"] = *(new Musique("ressources/Musique/menu.wav"));
	m_musique["pause"] = *(new Musique("ressources/Musique/pause.wav"));
}

void MusicSystem::stopMusic(int fadeOut)
{
	Mix_FadeOutMusic(fadeOut);
}