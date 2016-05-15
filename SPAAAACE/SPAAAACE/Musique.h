/*
Auteur : Gabriel Bouchard
Buts: classe conteneur des fonctions de SDL pour la musique
*/
#pragma once

#include <SDL_mixer.h>
#include <string>


class Musique
{
private:
	Mix_Music* m_musique;
	std::string m_filePath;
	bool m_isPlaying;
public:
	Musique(std::string filepath);
	Musique();
	Musique(const Musique &musique);
	~Musique();

	bool isPlaying(){ return m_isPlaying; }
	void play(unsigned fadeIn = 0);
	void stop(unsigned fadeOut = 0);
	void pause();

	Musique& operator=(const Musique &musique);
};