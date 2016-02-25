#pragma once

#include "PositionComponent.h"
#include "Sprite.h"


class GraphicsComponent :
	public Component
{
public:
	GraphicsComponent();
	GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr);
	~GraphicsComponent();

	std::shared_ptr<PositionComponent> getPositionComponent();
	Vec2 getPosition();

	std::shared_ptr<Sprite> getSprite() const;
	void setSprite(std::shared_ptr<Sprite> spr);

protected:
	//sprite
	//pointeur sur un renderer/fenetre
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<PositionComponent> m_posComponent;
	//SPECIALFX`: halo, particules etc
};

