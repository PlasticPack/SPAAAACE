#pragma once
#include "Component.h"
#include <vector>
#include <memory>

class GameLogicComponent :
	public Component
{
public:
	GameLogicComponent(luabridge::LuaRef& componentTable);
	virtual ~GameLogicComponent();

	unsigned int getCurrentLife() { return m_life; }
	unsigned int getMaxLife() { return m_maxLife; }
	unsigned int getMaxFuel() { return m_maxFuel; }
	unsigned int getCurrentFuel() { return m_fuel; }
	unsigned int getEnginePower() { return m_enginePower; }

	void setLife(unsigned int l) { m_life = l; }
	void setFuel(unsigned int l) { m_fuel = l; }
	void setEnginePower(unsigned int l) { m_enginePower = l; }

protected:
	unsigned int m_life;
	unsigned int m_maxLife;
	unsigned int m_fuel;
	unsigned int m_maxFuel;
	unsigned int m_enginePower;
};

