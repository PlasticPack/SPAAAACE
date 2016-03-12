#pragma once

#include <iostream>
#include "Message.h"
#include <memory>

class Component
{
public:
	Component();
	virtual ~Component();
	virtual void update() {}
};

