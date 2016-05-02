#pragma once
#include "Message.h"
#include "GameObject.h"
#include "Vec2.h"

#include <fstream>
#include "Command.h"
#include <queue>
class Cinematique
{
protected:
	bool m_isPlaying;
	bool m_isDone;

public:
	
	Cinematique();
	~Cinematique();
	bool isPlaying() { return m_isPlaying; };
	bool isDone() { return m_isDone; }
	void setPlaying(bool isPlaying) { m_isPlaying = isPlaying; } 
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
	CinematiqueMort();
	~CinematiqueMort();
	bool virtual update(GameObject* objet, Message &postman);
};

class CinematiqueIntro : public Cinematique
{
private:
	CommandStop m_stop;
	CommandText m_text;
public:
	CinematiqueIntro();
	~CinematiqueIntro();
	bool virtual update(GameObject* objet, Message &postman);
};

class CinematiqueFirstBase : public Cinematique
{
private:
	CommandStop m_stop;
	CommandText m_text;
public:
	CinematiqueFirstBase();
	~CinematiqueFirstBase();
	bool virtual update(GameObject* objet, Message &postman);
};