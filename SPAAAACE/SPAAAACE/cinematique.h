#pragma once
#include "Message.h"
#include "GameObject.h"
#include "Vec2.h"

#include <fstream>
#include "Command.h"
#include <queue>
class Cinematique
{
private:
	bool m_isPlaying;
	

public:
	
	Cinematique();
	~Cinematique();
	bool isPlaying() { return m_isPlaying; };
	bool virtual update(GameObject* objet,Message &postman);

	

};

class CinematiqueMort : public Cinematique
{
private:
	CommandRevSpeed m_rev;
	CommandRevSpeed m_rev2;
	CommandStop m_stop;
	CommandText m_text;
public:
	bool virtual update(GameObject* objet, Message &postman);
};