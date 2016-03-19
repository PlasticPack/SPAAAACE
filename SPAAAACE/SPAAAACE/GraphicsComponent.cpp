#include "GraphicsComponent.h"
#include <LuaBridge.h>

GraphicsComponent::GraphicsComponent()
{
	m_posComponent = nullptr;
	m_sprite = nullptr;
	m_center = Vec2(0, 0);
	m_size = Vec2(0, 0);
	m_camera = true;
}


GraphicsComponent::GraphicsComponent(luabridge::LuaRef& componentTable, std::shared_ptr<PositionComponent> comp){
	GraphicsComponent();
	using namespace luabridge;
	auto spriteRef = componentTable["filename"];
	auto cols = componentTable["c"];
	auto rows = componentTable["r"];
	auto anim_speed = componentTable["anim_speed"];
	auto cam = componentTable["cam"];
	auto width = componentTable["width"];
	auto height = componentTable["height"];

	m_camera = true;
	if (spriteRef.isString()){

		m_sprite = std::make_shared<Sprite>(std::make_shared<SpriteSheet>(GraphicsSystem::loadTexture(spriteRef), cols.cast<int>(), rows.cast<int>()));

		if (anim_speed.isString()){
			m_sprite->setAnimationSpeed(anim_speed);
		}

		if (cam.isString() && (cam == "true" || cam == "false")){
			if (cam == "true")
				m_camera = true;
			else
				m_camera = false;
		}

		if (width.isNumber() && height.isNumber()){
			m_size = Vec2(width, height);
		}
		else {
			m_size = Vec2(m_sprite->getCurrentSpriteSheet()->getCurrentRect().w, m_sprite->getCurrentSpriteSheet()->getCurrentRect().h);
		}

		m_center = Vec2(m_size.x() / 2, m_size.y() / 2);
		
	}
	else{

		setSprite(nullptr);
	}


	this->m_posComponent = comp;
}



GraphicsComponent::GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr)
{
	m_camera = true;
	m_sprite = spr;
	m_center = Vec2(spr->getCurrentSpriteSheet()->getCurrentRect().w / 2, spr->getCurrentSpriteSheet()->getCurrentRect().h / 2);
	//std::cout << m_center.x() << "  TWAS THE CENTER :D \n";
	m_posComponent = comp;
	m_size = Vec2(spr->getCurrentSpriteSheet()->getCurrentRect().w, spr->getCurrentSpriteSheet()->getCurrentRect().h);
}

void GraphicsComponent::setSprite(std::shared_ptr<Sprite> spr){
	m_sprite = spr;
}

GraphicsComponent::~GraphicsComponent()
{
}

void GraphicsComponent::setAnimationSpeed(double s){
	m_sprite->setAnimationSpeed(s);
}

Vec2 GraphicsComponent::getSize(){
	return m_size;
}

void GraphicsComponent::setSize(Vec2 s){
	m_size = s;
}

Vec2 GraphicsComponent::getCenter(){
	return m_center;
}

std::shared_ptr<PositionComponent> GraphicsComponent::getPositionComponent(){
	return m_posComponent;
}

std::shared_ptr<Sprite> GraphicsComponent::getSprite() const{
	return m_sprite;
}

Vec2 GraphicsComponent::getPosition(){
	return m_posComponent->getPosition();
}

void GraphicsComponent::setPositionComponent(std::shared_ptr<PositionComponent> pcomp=nullptr){
	this->m_posComponent = pcomp;
}