#include "PositionComponent.h"
#include <LuaBridge.h>

PositionComponent::PositionComponent() : Component()
{
	std::cout << "PCOMP BUILT\n";
	m_zIndex = 1.0;
}

PositionComponent::PositionComponent(luabridge::LuaRef& componentTable){
	PositionComponent();
	using namespace luabridge;
	auto x = componentTable["x"];
	auto y = componentTable["y"];

	if (x.isNumber() && y.isNumber()){
		m_position.setCoords(x, y);
	}

	//Nothing to set here...
}


PositionComponent::~PositionComponent()
{
	//std::cout << "\nPCOMP DESTROYED";
}


void PositionComponent::update()
{
	std::cout << "\nUPDATING POSITION";
}
