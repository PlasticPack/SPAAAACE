#include "PositionComponent.h"
#include <LuaBridge.h>

PositionComponent::PositionComponent() : Component()
{
	//std::cout << "PCOMP BUILT\n";
	m_zIndex = 1.0;
}

PositionComponent::PositionComponent(luabridge::LuaRef& componentTable){
	PositionComponent();
	m_zIndex = 1.0;
	m_angle = 0;
	m_position = Vec2(0, 0);
	using namespace luabridge;
	auto x = componentTable["x"];
	auto y = componentTable["y"];
	auto z = componentTable["z"];
	auto a = componentTable["a"];

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
