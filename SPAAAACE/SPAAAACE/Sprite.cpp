#include "Sprite.h"


Sprite::Sprite()
{
	std::cout << "DEFAULT SPRITE " << this << " CREATED\n";
	m_currentSpriteSheet = "default";
}

Sprite::Sprite(std::shared_ptr<SpriteSheet> sprS)
{
	m_spriteSheets["default"] = sprS;
	std::cout << "custom SPRITE " <<  this << " CREATED\n";
	m_currentSpriteSheet = "default";
}

Sprite::~Sprite()
{
	std::cout << "Destroying Sprite " << this << " with SprSheet texture  " <<  m_spriteSheets[m_currentSpriteSheet]->getTexture() << "\n";
}

void Sprite::addSpriteSheet(std::string id, std::shared_ptr<SpriteSheet> sheet){
	m_spriteSheets[id] = sheet;
	//std::cout << "ADDING " << id << " SPRITESHEET with texture address " << sheet->getTexture() << " vs here " <<  m_spriteSheets[id]->getTexture() << "\n";
	std::cout << m_spriteSheets[id]->getCurrentRect().w << " IT'S A TRAP\n";
}

std::shared_ptr<SpriteSheet> Sprite::getCurrentSpriteSheet() {
	//std::cout << "Current SprSheet " << m_currentSpriteSheet << "\n";
	return m_spriteSheets[m_currentSpriteSheet];
}

void Sprite::setAnimationSpeed(double s){
	m_spriteSheets[m_currentSpriteSheet]->setAnimationSpeed(s);
}