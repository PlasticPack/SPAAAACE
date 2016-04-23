#include "Sprite.h"


Sprite::Sprite()
{
	m_animationSpeed = 1;
	//std::cout << "DEFAULT SPRITE " << this << " CREATED\n";
	m_currentSpriteSheet = "default";
}

Sprite::Sprite(std::shared_ptr<SpriteSheet> sprS)
{
	m_animationSpeed = 1;
	m_spriteSheets["default"] = sprS;
	//std::cout << "custom SPRITE " <<  this << " CREATED\n";
	m_currentSpriteSheet = "default";
}

Sprite::~Sprite()
{
	//std::cout << "Destroying Sprite " << this << " with SprSheet texture  " <<  m_spriteSheets[m_currentSpriteSheet]->getTexture() << "\n";
}

void Sprite::addSpriteSheet(std::string id, std::shared_ptr<SpriteSheet> sheet){
	m_spriteSheets[id] = sheet;
	//std::cout << "ADDING " << id << " SPRITESHEET\n";
	//std::cout << m_spriteSheets[id]->getCurrentRect().w << " IT'S A TRAP\n";
}

std::shared_ptr<SpriteSheet> Sprite::getSpriteSheet(std::string name){

	return m_spriteSheets[name];
}

std::shared_ptr<SpriteSheet> Sprite::getCurrentSpriteSheet() {

	return m_spriteSheets[m_currentSpriteSheet];
}

void Sprite::setAnimationSpeed(double s){
	m_animationSpeed = s;
	m_spriteSheets[m_currentSpriteSheet]->setAnimationSpeed(s);
}

void Sprite::setSpriteSheet(std::string id){
	
	if (m_spriteSheets[id] != nullptr){
		//std::cout << m_animationSpeed << "\n";
		m_spriteSheets[id]->setAnimationSpeed(m_animationSpeed);
		m_currentSpriteSheet = id;
	}
	//else
		//std::cout << "sprsheet " << id << " not found\n";
}