#include "Musique.h"

Musique::Musique(std::string filepath)
{
	m_musique = Mix_LoadMUS(filepath.c_str());
	m_filePath = filepath;
}

Musique::Musique(const Musique &musique)
{
	*this = musique;
}

Musique& Musique::operator=(const Musique &musique)
{
	m_musique = Mix_LoadMUS(musique.m_filePath.c_str());
	m_filePath = musique.m_filePath;
	return *this;
}

Musique::Musique()
{
	m_musique = NULL;
}
Musique::~Musique()
{	
	Mix_FreeMusic(m_musique);
	m_musique = NULL;
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