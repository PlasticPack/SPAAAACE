#pragma once

#include <iostream>
#include "Message.h"
#include <memory>

namespace luabridge {
	class LuaRef;
}

class Component
{
public:
	Component();
	virtual ~Component();
	virtual void update() {}
};

