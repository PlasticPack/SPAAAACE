
/********************
Créé par Tristan Roy
Entité abstraire représentant 
une partie d'un objet
Ne contient que des données, 
qui elles sont manipulées et 
modifiées par les systèmes
correspondants
*********************/
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

