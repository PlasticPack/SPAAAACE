/*
Créé par Charles Murphy
Permet de gérer une position spatiale en x, y, et "z"
z : layer (double)
indique la hauteur absolue
zIndex 0 : base, loin "à l'infini", ne bouge apparament pas
zIndex 1 : layer standard, par défaut tout est à ce niveau
zIndex > 1 : plus "proche" de la caméra
zIndex < 1 && > 0 : plus loin de la caméra
*/
#pragma once
#include "Component.h"
#include "Vec2.h"

class Component;

class PositionComponent : public Component {
public:
	PositionComponent();
	PositionComponent(luabridge::LuaRef& componentTable);

	virtual ~PositionComponent();
	void update();

	double getAngle() { return m_angle; }


	void setPosition(Vec2 pos) { m_position = pos;  }
	void setZIndex(double z) { if(z >= 0) 
		m_zIndex = z; }

	void setAngle(double a) { m_angle = a; }

	Vec2 getPosition() { return m_position; }
	double getZIndex() { return m_zIndex; }

	bool isFixed(){ return m_fixed; }
	void fix(bool f) { m_fixed = f; }

	bool operator<(const PositionComponent &comp);

protected:
	Vec2 m_position;
	double m_angle;
	double m_zIndex; 
	bool m_fixed;
};

