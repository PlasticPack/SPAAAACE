#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Vec2.h"
#include <iostream>

/***********************************
Classe qui gère un ensemble d'images
constituant une animation 
************************************/

class SpriteSheet
{
public:
	SpriteSheet(SDL_Texture *texture, SDL_Rect rectSize, SDL_Rect sheetSize);
	~SpriteSheet();
	
	const SDL_Rect getCurrentRect();
	SDL_Texture *getTexture();
	void setAnimationSpeed(double d);
	void nextRect(double dt);

protected:
	double m_animationSpeed; //vitesse d'animation en images par seconde
	double m_timer;
	SDL_Texture *m_texture;
	SDL_Rect m_sheetSize;
	SDL_Rect m_currentRect;
	/*
	*Type de blend:
	*	SDL_BLENDMODE_NONE
	*	SDL_BLENDMODE_BLEND
	*	SDL_BLENDMODE_ADD
	*	SDL_BLENDMODE_MOD
	*/
	void setBlendMode(SDL_BlendMode blendModeFlag);
	void setColorAlpha(Uint8 alpha);
	void setColorMod(Uint8 red, Uint8 green, Uint8 blue);
};

