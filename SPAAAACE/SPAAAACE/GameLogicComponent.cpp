#include "GameLogicComponent.h"
#include <LuaBridge.h>


GameLogicComponent::GameLogicComponent(luabridge::LuaRef& componentTable)
{
	using namespace luabridge;

	m_enginePower = 10;
	m_maxFuel = 0;
	m_life = 3;

	auto pwr =	   componentTable["pwr"];
	auto maxLife = componentTable["life"];
	auto maxFuel = componentTable["fuel"];

	if (pwr.isNumber())
		m_enginePower = pwr;

	if (maxFuel.isNumber())
		m_maxFuel = maxFuel;

	if (maxLife.isNumber())
		m_maxLife = maxLife;


	m_life = m_maxLife;
	m_fuel = m_maxFuel;

	std::cout << m_fuel << " is the fuel\n";
}


GameLogicComponent::~GameLogicComponent()
{
}
