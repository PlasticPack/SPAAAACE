#include "PositionComponent.h"
#include <LuaBridge.h>

PositionComponent::PositionComponent() : Component()
{
	//std::cout << "PCOMP BUILT\n";
	m_zIndex = 1.0;
	m_fixed = false;
	m_position = Vec2(0, 0);
}

PositionComponent::PositionComponent(luabridge::LuaRef& componentTable){
	PositionComponent();
	m_zIndex = 1.0;
	m_angle = 0;
	m_position = Vec2(0, 0);
	m_fixed = false;
	using namespace luabridge;
	auto x = componentTable["x"];
	auto y = componentTable["y"];
	auto z = componentTable["z"];
	auto a = componentTable["a"];
	auto fixed = componentTable["fixed"];

	if (fixed.isString() && (fixed == "true" || fixed == "false")){
		if (fixed == "true"){
			m_fixed = true;
		}
		else {
			m_fixed = false;
		}
	}

	if (x.isNumber() && y.isNumber()){
		m_position.setCoords(x, y);
	}

	if (z.isNumber()){
		m_zIndex = z;
	}

	if (a.isNumber()){
		m_angle = a;
	}
}


PositionComponent::~PositionComponent()
{
	//std::cout << "\nPCOMP DESTROYED";
}


void PositionComponent::update()
{
	std::cout << "\nUPDATING POSITION";
}


bool PositionComponent::operator<(const PositionComponent &comp){

	if (&comp != this){
		return m_zIndex < comp.m_zIndex;
	}
	return false;
}