/*
Auteur : Gabriel Bouchard
Buts: classe conteneur des fonctions de SDL pour la musique
*/

#include "Musique.h"

Musique::Musique(std::string filepath) : m_isPlaying(false)
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
	m_isPlaying = musique.m_isPlaying;
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
	if (!m_isPlaying)
	{
		m_isPlaying = true;
		if (Mix_PausedMusic()) Mix_ResumeMusic();
		else
		Mix_FadeInMusic(m_musique,-1,fadeIn);
	}
}

void Musique::stop(unsigned fadeOut)
{
	Mix_FadeOutMusic(fadeOut);
	m_isPlaying = false;
}

void Musique::pause()
{
	Mix_PauseMusic();
}