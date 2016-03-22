#pragma once


#include "Component.h"
#include "Flags.h"
#include <memory>

class InputsSystem;
class PhysicsComponent;


class InputsComponent :
	public Component
{
public:
	
	//InputsComponent(std::shared_ptr<InputsSystem> inputs, std::shared_ptr<PhysicsComponent> physComp=nullptr);
	InputsComponent(luabridge::LuaRef& componentTable);
	
	~InputsComponent();

	void update();

	void setPhysComp(std::shared_ptr<PhysicsComponent> comp=nullptr){this->m_physComp = comp;}
protected:
	//sprite
	//pointeur sur un renderer/fenetre
	std::shared_ptr<InputsSystem> m_inputs;
	std::shared_ptr<PhysicsComponent> m_physComp;
	//SPECIALFX`: halo, particules etc

};