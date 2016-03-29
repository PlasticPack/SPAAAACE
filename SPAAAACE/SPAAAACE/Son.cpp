#include "Son.h"
#include <iostream>
using namespace std;

Son::Son()
{
	m_son = NULL;
}

Son::Son(string filepath)
{
	m_son = Mix_LoadWAV(filepath.c_str());
	if (m_son == NULL)
	{
		cout << "Erreur load Son : " << filepath << endl
			<<"\tErreur SDL:"<< Mix_GetError() << endl;
	}
	
}
Son::~Son()
{
	Mix_FreeChunk(m_son);
}

void Son::play(int distance, int angle, int loop)
{
	int channel = Mix_PlayChannel(-1, m_son, loop);

	Mix_SetPosition(channel, angle, distance);
}