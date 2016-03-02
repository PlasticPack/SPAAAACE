#pragma once

#include <iostream>
#include <memory>

class Component
{
public:
	Component();
	virtual ~Component();
	virtual void update() {}
};

