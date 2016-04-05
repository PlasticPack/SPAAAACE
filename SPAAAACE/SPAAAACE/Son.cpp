#include "Son.h"
#include <iostream>
using namespace std;

Son::Son()
{
	m_son = NULL;
	m_channel = -1;
}

Son::Son(string filepath, int channel)
{
	m_son = Mix_LoadWAV(filepath.c_str());
	if (m_son == NULL)
	{
		cout << "Erreur load Son : " << filepath << endl
			<<"\tErreur SDL:"<< Mix_GetError() << endl;
	}
	m_channel = channel;
}
Son::~Son()
{
	Mix_FreeChunk(m_son);
	m_son = NULL;
}

void Son::play(int distance, int angle, int loop)
{

	if (!Mix_Playing(m_channel))
	{
		Mix_SetPosition(m_channel, angle, distance);
		Mix_PlayChannel(m_channel, m_son, loop);
	}

}