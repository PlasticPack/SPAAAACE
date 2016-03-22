#include "InputsComponent.h"
#include "InputSystem.h"
#include "PhysicsComponent.h"
#include "Vec2.h"

using namespace std;

InputsComponent::InputsComponent(luabridge::LuaRef& componentTable): Component(){
	//nothing for now
}


InputsComponent::~InputsComponent()
{

}

void InputsComponent::update()
{
	//Vec2 pushForce;
	//pushForce.setX(m_inputs->checkTriggeredAction(AC_HORIZONTAL_PUSH) / 32768 * m_physComp->getPushValue());
	//pushForce.setY(m_inputs->checkTriggeredAction(AC_VERTICAL_PUSH) / 32768 * m_physComp->getPushValue());
	//m_physComp->setForces(pushForce);
	
}
