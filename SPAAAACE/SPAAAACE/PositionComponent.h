#pragma once
#include "Component.h"
#include "Vec2.h"

/*Permet de g�rer une position spatiale en x, y, et "z"
z : layer (double)
indique la hauteur absolue
zIndex 0 : base, loin "� l'infini", ne bouge apparament pas
zIndex 1 : layer standard, par d�faut tout est � ce niveau
zIndex > 1 : plus "proche" de la cam�ra
zIndex < 1 && > 0 : plus loin de la cam�ra
*/
class Component;

class PositionComponent : public Component {
public:
	PositionComponent();
	PositionComponent(luabridge::LuaRef& componentTable);

	virtual ~PositionComponent();
	void update();

	void setPosition(Vec2 pos) { m_position = pos;  }
	void setZIndex(double z) { if(z >= 0) 
		m_zIndex = z; }

	Vec2 getPosition() { return m_position; }
	double getZIndex() { return m_zIndex; }

protected:
	Vec2 m_position;
	double m_zIndex; 
};

