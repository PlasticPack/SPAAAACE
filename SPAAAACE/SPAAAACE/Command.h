/*
Auteur : Gabriel Bouchard
Buts: Contient toutes les commandes utiliser dans les cinématiques et ailleur dans le jeu.
*/

#pragma once
#include "GameObject.h"
#include <math.h>
#include "PhysicsComponent.h"
#include <string>
#include "GraphicsSystem.h"
//Virtual
class Command
{
protected :
	bool m_isDone;
public:
	Command():m_isDone(false){}
	~Command(){}

	bool isDone() { return m_isDone; }
	void setIsDone(bool isDone) { m_isDone = isDone; }

	virtual bool execute(GameObject* objet) = 0;

};

class CommandStop : public Command
{
private:
	double m_acc;
public:
	CommandStop() : Command()
	{
		m_acc = 100;
	}
	CommandStop(double acc) : Command()
	{
		m_acc = acc;
	}
	~CommandStop(){}

	void setAcc(double acc) { m_acc = acc; }

	bool execute(GameObject* objet)
	{
		Vec2 vitesse = objet->get<PhysicsComponent>()->getVelocity();
		double length = vitesse.getLength() - m_acc;

		if (length <= 0)
		{
			vitesse *= 0;
			objet->get<PhysicsComponent>()->setVelocity(vitesse);
			m_isDone = true;
			return true;
		}
		vitesse.normalize();
		vitesse *= length;
		objet->get<PhysicsComponent>()->setVelocity(vitesse);
		return false;
	}
};

class CommandRevSpeed : public Command
{
private:
	double m_acc;
	double m_speed;
public:
	CommandRevSpeed() : Command()
	{
		m_acc = 100;
		m_speed = 0;
	}
	CommandRevSpeed(double speed, double acc) : Command()
	{
		m_speed = speed;
		m_acc = acc;
	}
	~CommandRevSpeed(){}

	void setAcc(double acc) { m_acc = acc; }

	bool execute(GameObject* objet)
	{
		double vitesse = objet->get<PhysicsComponent>()->getAngularVelocity();
		if (vitesse < m_speed)
			vitesse += m_acc;
		else
			vitesse -= m_acc;
		objet->get<PhysicsComponent>()->setAngularVelocity(vitesse);

		if (abs(abs(vitesse) - abs(m_speed)) < m_acc/2)
		{
			vitesse  = m_speed;
			m_isDone = true;
			return true;
		}
		
		return false;
	}
};

class CommandText : public Command
{
private:
	std::string* m_text;
	int m_size;
	bool m_isInitialzed;
public:
	
	CommandText(std::string text[], int size) : Command()
	{
		m_isInitialzed = false;
		m_text = text;
		m_size = size;
	}
	CommandText() : Command()
	{
		m_isInitialzed = false;
		m_text;
	}
	~CommandText()
	{

	}
	virtual bool execute(GameObject* objet)
	{
		if (!m_isInitialzed)
		{
			for (int i = 0; i < m_size;i++)
			GraphicsSystem::print(m_text[i]);
			m_isInitialzed = true;
		}
		else if (!GraphicsSystem::isTextLeft())
		{
			m_isDone = true;
			m_isInitialzed = false;
			return true;
		}
		return false;
	}
};

class CommandGravity : public Command
{
private:
	int m_time;
	int m_mass;
	int m_ancientMass;
	bool m_isInit;
public:

	CommandGravity() : Command(), m_isInit(false)
	{
		m_mass = 2500;
	}
	CommandGravity(int mass) : Command(), m_isInit(false)
	{
		m_mass = mass;
	}
	virtual bool execute(GameObject* objet)
	{
		
		if (!m_isInit)
		{
			m_ancientMass = objet->get<PhysicsComponent>()->getMass();
			m_isInit = true;
		}

		if (m_mass == objet->get<PhysicsComponent>()->getMass())
		{
			m_isInit = false;
			objet->get<PhysicsComponent>()->setMass(m_ancientMass);
		}
		else
			objet->get<PhysicsComponent>()->setMass(m_mass);
		
		return true;
	}
};

class CommandChangeSprite : public Command
{
private:
	std::string m_animationSprite;

public:

	CommandChangeSprite() : Command()
	{
		m_animationSprite = "default";
	}
	CommandChangeSprite(std::string animation) : Command()
	{
		m_animationSprite = animation;
	}
	virtual bool execute(GameObject* objet)
	{
		objet->get<GraphicsComponent>()->getSprite()->setSpriteSheet(m_animationSprite);
		m_isDone = true;
		return true;
	}

};