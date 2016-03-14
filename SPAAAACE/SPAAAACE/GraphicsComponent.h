#pragma once

#include "PositionComponent.h"
#include "Sprite.h"

struct C_Color{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

class GraphicsComponent :
	public Component
{
public:
	GraphicsComponent();
	GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr);
	GraphicsComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PositionComponent> comp);
	~GraphicsComponent();

	std::shared_ptr<PositionComponent> getPositionComponent();
	Vec2 getPosition();
	Vec2 getCenter();

	std::shared_ptr<Sprite> getSprite() const;
	void setSprite(std::shared_ptr<Sprite> spr);

	void setAnimationSpeed(double s);
	void setPositionComponent(std::shared_ptr<PositionComponent>);

	Vec2 getSize();
	void setSize(Vec2 s);

	//HALO 
	//pas utiliser pour le moment
	void setHaloColor(C_Color c);
	void activateHalo(bool halo = true);

	C_Color getHaloColor();
	bool isHaloOn();

protected:
	//sprite
	//pointeur sur un renderer/fenetre
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<PositionComponent> m_posComponent;
	C_Color m_halo;
	bool m_haloOn;
	Vec2 m_size;
	Vec2 m_center;

	//SPECIALFX`: halo, particules etc
};

