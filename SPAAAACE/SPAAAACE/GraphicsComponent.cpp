#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent()
{
	m_posComponent = nullptr;
	m_sprite = nullptr;
	m_halo = { 255, 25, 25, 255};
	m_haloOn = false;
	m_center = Vec2(0, 0);
	m_size = Vec2(0, 0);
}

GraphicsComponent::GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr)
{
	m_halo = { 255, 25, 25, 255 };
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

void GraphicsComponent::setHaloColor(C_Color c){
	m_halo = c;
}

void GraphicsComponent::activateHalo(bool h){
	m_haloOn = h;
}

C_Color GraphicsComponent::getHaloColor(){
	return m_halo;
}

bool GraphicsComponent::isHaloOn(){
	return m_haloOn;
}