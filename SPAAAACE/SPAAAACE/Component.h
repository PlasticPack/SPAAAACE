#pragma once

#include <iostream>
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

