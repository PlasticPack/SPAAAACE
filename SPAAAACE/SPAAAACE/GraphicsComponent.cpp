#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent()
{
	m_posComponent = nullptr;
	m_sprite = nullptr;
}

GraphicsComponent::GraphicsComponent(std::shared_ptr<PositionComponent> comp, std::shared_ptr<Sprite> spr)
{
	m_sprite = spr;
	m_posComponent = comp;
}

void GraphicsComponent::setSprite(std::shared_ptr<Sprite> spr){
	m_sprite = spr;
}

GraphicsComponent::~GraphicsComponent()
{
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