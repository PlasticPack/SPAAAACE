#pragma once
#include <SDL_mixer.h>
#include <string>

class Son
{
private:
	Mix_Chunk* m_son;
	int m_channel;
public:
	Son(std::string filepath, int channel = -1);
	Son();
	~Son();

	void play(int distance = 0, int angle = 0, int loop = 0);

};