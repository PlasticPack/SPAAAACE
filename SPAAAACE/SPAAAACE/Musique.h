#pragma once

#include <SDL_mixer.h>
#include <string>


class Musique
{
private:
	Mix_Music* m_musique;
	std::string m_filePath;
public:
	Musique(std::string filepath);
	Musique();
	Musique(const Musique &musique);
	~Musique();

	void play(unsigned fadeIn = 0);
	void stop(unsigned fadeOut = 0);
	void pause();

	Musique& operator=(const Musique &musique);
};