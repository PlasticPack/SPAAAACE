#include "Musique.h"

Musique::Musique(std::string filepath)
{
	m_musique = Mix_LoadMUS(filepath.c_str());
}


Musique::~Musique()
{
	Mix_FreeMusic(m_musique);
}

void Musique::play(unsigned fadeIn)
{
	if (Mix_PausedMusic()) Mix_ResumeMusic();
	else
	Mix_FadeInMusic(m_musique,-1,fadeIn);
}

void Musique::stop(unsigned fadeOut)
{
	Mix_FadeOutMusic(fadeOut);
}

void Musique::pause()
{
	Mix_PauseMusic();
}