#pragma once
#include <SDL_mixer.h>
#include <string>

class Son
{
private:
	Mix_Chunk* m_son;
public:
	Son(std::string filepath);
	Son();
	~Son();

	void play(int distance = 0, int angle = 0, int loop = 0);

};