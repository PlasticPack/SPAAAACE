#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Vec2.h"
#include <iostream>
#include <memory>

/***********************************
Classe qui gère un ensemble d'images
constituant une animation
************************************/

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(SDL_Texture* texture, int nbcol = 1, int nbrow = 1);
	SpriteSheet(SDL_Texture* texture, SDL_Rect rectSize, SDL_Rect);
	SpriteSheet(SpriteSheet const&);
	SpriteSheet& operator=(SpriteSheet const&);
	~SpriteSheet();

	const SDL_Rect getCurrentRect();
	SDL_Texture* getTexture();
	void setAnimationSpeed(double d);
	void nextRect(double dt);
	std::string getFilename() const { return m_filename; }
	void setFilename(std::string f) { m_filename = f; }

	int getCurrentRectIndex(){
		std::cout << m_currentRectIndex << "\n";
		return m_currentRectIndex;
	}

	int getTotalRectNumber(){
		//std::cout << "   \n"<< ((m_sheetSize.w) / (m_currentRect.w)) * ((m_sheetSize.h) / (m_currentRect.h)) - 1;
		return ((m_sheetSize.w) / (m_currentRect.w)) * ((m_sheetSize.h) / (m_currentRect.h));
	}

protected:
	double m_animationSpeed; //vitesse d'animation en images par seconde
	double m_timer;
	SDL_Texture* m_texture;
	SDL_Rect m_sheetSize;
	SDL_Rect m_currentRect;
	std::string m_filename;
	int m_currentRectIndex;
};
