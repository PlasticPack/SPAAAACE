/********************
Cr�� par Tristan Roy
Entit� abstraire repr�sentant 
une partie d'un objet
Ne contient que des donn�es, 
qui elles sont manipul�es et 
modifi�es par les syst�mes
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

