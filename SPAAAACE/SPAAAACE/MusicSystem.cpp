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
}

MusicSystem::~MusicSystem()
{
	Mix_CloseAudio();
}

void MusicSystem::update()
{
	m_son["explosion"].play();
}

void MusicSystem::init()
{
	m_son["explosion"] = *(new Son("C:/Users/bouchagb/Desktop/Explosion23.wav"));

}