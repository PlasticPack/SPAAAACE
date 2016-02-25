#include "PositionComponent.h"


PositionComponent::PositionComponent() : Component()
{
	std::cout << "PCOMP BUILT\n";
	m_zIndex = 1.0;
}


PositionComponent::~PositionComponent()
{
	//std::cout << "\nPCOMP DESTROYED";
}


void PositionComponent::update()
{
	std::cout << "\nUPDATING POSITION";
}
