#pragma once
#include "SpriteSheet.h"

const int SCREEN_W = 1600;
const int SCREEN_H = 900;

struct Camera{
	Vec2 target;
	double angle;
	double zoom;
};

namespace Graphics{

	SDL_Texture* loadTexture(const std::string filename, SDL_Renderer *ren);
	
	void drawSpriteSheet(SpriteSheet &sprSht, double dt, SDL_Renderer *ren, Vec2 coord, Camera cam);

	
}