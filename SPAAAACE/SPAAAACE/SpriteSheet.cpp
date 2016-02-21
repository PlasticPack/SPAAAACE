#include "SpriteSheet.h"


SpriteSheet::SpriteSheet(SDL_Texture *texture, SDL_Rect rectSize, SDL_Rect sheetSize) : m_sheetSize(sheetSize)
{
	m_texture = texture;
	m_animationSpeed = 1;
	m_timer = 0;
	m_currentRect = SDL_Rect{ 0, 0, rectSize.w, rectSize.h };
}

void SpriteSheet::nextRect(double dt){
	//si on a pris assez de temps pour la frame
	m_timer += dt;

	if (m_timer > 1.0/m_animationSpeed){
		m_timer = 0;
		m_currentRect.x += m_currentRect.w;
		
		if (m_currentRect.x + m_currentRect.w > m_sheetSize.w){
			m_currentRect.x = 0;

			m_currentRect.y += m_currentRect.h;

			if (m_currentRect.y + m_currentRect.h > m_sheetSize.h){
				m_currentRect.y = 0;
			}
		}
	}

}

void SpriteSheet::setAnimationSpeed(double d){
	m_animationSpeed = d;
}

const SDL_Rect SpriteSheet::getCurrentRect(){
	return m_currentRect;
}

SDL_Texture* SpriteSheet::getTexture(){
	return m_texture;
}

void SpriteSheet::setColorMod(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}
void SpriteSheet::setColorAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

void SpriteSheet::setBlendMode(SDL_BlendMode blendModeFlag)
{

	SDL_SetTextureBlendMode(m_texture, blendModeFlag);
}

SpriteSheet::~SpriteSheet()
{
	SDL_DestroyTexture(m_texture);
}
