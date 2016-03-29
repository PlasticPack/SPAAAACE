#pragma once

#include <SDL_mixer.h>
#include <string>


class Musique
{
private:
	Mix_Music* m_musique;
public:
	Musique(std::string filepath);
	~Musique();

	void play(unsigned fadeIn = 0);
	void stop(unsigned fadeOut = 0);
	void pause();
};