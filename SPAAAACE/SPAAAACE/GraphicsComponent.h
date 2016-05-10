/*************************
Créé par Charles Murphy
Component servant à afficher un objet
et à l'animer
**************************/
#pragma once

#include "PositionComponent.h"
#include "GraphicsSystem.h"
#include "Sprite.h"
#include <ctype.h>

struct C_Color{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

class GraphicsSystem;

class GraphicsComponent :
	public Component
{
public:
	GraphicsComponent();
	GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr);
	GraphicsComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PositionComponent> comp=nullptr);
	~GraphicsComponent();

	GraphicsComponent(GraphicsComponent const& g);
	GraphicsComponent& operator=(GraphicsComponent const& g);

	std::shared_ptr<PositionComponent> getPositionComponent();
	Vec2 getPosition();
	Vec2 getCenter();

	std::shared_ptr<Sprite> getSprite() const;
	void setSprite(std::shared_ptr<Sprite> spr);

	void setAnimationSpeed(double s);
	void setPositionComponent(std::shared_ptr<PositionComponent>);

	Vec2 getSize();
	Vec2 getMaxSize();
	void setSize(Vec2 s);

	void affectedByCamera(bool c) { m_camera = c; };
	bool isAffectedByCamera(){ return m_camera; }

	std::string getFilename() { return m_sprite->getCurrentSpriteSheet()->getFilename(); }

protected:
	//sprite
	//pointeur sur un renderer/fenetre
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<PositionComponent> m_posComponent;
	Vec2 m_size;
	Vec2 m_maxSize;
	Vec2 m_center;
	bool m_camera;
	//SPECIALFX`: halo, particules etc
};

