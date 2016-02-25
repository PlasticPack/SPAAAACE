#pragma once

#include "SpriteSheet.h"
#include <map>

class Sprite
{
public:
	Sprite();
	Sprite(std::shared_ptr<SpriteSheet> sprS);
	~Sprite();

	void addSpriteSheet(std::string id, std::shared_ptr<SpriteSheet> sheet);
	std::shared_ptr<SpriteSheet> getCurrentSpriteSheet();

protected:
	std::map<std::string, std::shared_ptr<SpriteSheet>> m_spriteSheets;
	std::string m_currentSpriteSheet;
};

