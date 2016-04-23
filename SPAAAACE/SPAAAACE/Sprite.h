#pragma once

#include "SpriteSheet.h"
#include <map>

class Sprite
{
public:
	Sprite();
	Sprite(std::shared_ptr<SpriteSheet> sprS);
	~Sprite();

	void setAnimationSpeed(double s);
	void addSpriteSheet(std::string id, std::shared_ptr<SpriteSheet> sheet);
	void setSpriteSheet(std::string id);
	std::shared_ptr<SpriteSheet> getCurrentSpriteSheet();


	std::shared_ptr<SpriteSheet> getSpriteSheet(std::string name);

protected:
	double m_animationSpeed; // en fps
	std::map<std::string, std::shared_ptr<SpriteSheet>> m_spriteSheets;
	std::string m_currentSpriteSheet;
};

