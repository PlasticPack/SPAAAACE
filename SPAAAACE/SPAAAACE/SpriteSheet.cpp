#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(){
	m_texture = nullptr;
	m_animationSpeed = 1;
	m_timer = 0;
	m_currentRect = SDL_Rect{0,0,0,0};
	//std::cout << "DEFAULT SPRSHEET " << this << " CREATED\n";
}

SpriteSheet::SpriteSheet(SDL_Texture* texture, SDL_Rect rectSize, SDL_Rect sheetSize) : m_sheetSize(sheetSize)
{
	m_texture = texture;
	m_animationSpeed = 1;
	m_timer = 0;
	m_currentRect = SDL_Rect{ 0, 0, rectSize.w, rectSize.h };
	//std::cout << "CUSTOM SPRSHEET : " << this << " w/ texture " << texture <<  "\n";
}

SpriteSheet::SpriteSheet(SpriteSheet const& a){
	if (this != &a){
		//std::cout << "Copying " << &a << " to " << this << "\n";
		m_texture = a.m_texture; // pas besoin de faire de deep copy
		m_animationSpeed = a.m_animationSpeed;
		m_timer = a.m_timer;
		m_currentRect = a.m_currentRect;
	}
}

SpriteSheet& SpriteSheet::operator=(SpriteSheet const& a){
	if (this != &a){
		//std::cout << "Copying " << &a << " to " << this << "\n";
		m_texture = a.m_texture;
		m_animationSpeed = a.m_animationSpeed;
		m_timer = a.m_timer;
		m_currentRect = a.m_currentRect;
	}
	return *this;
}

void SpriteSheet::nextRect(double dt){
	//si on a pris assez de temps pour la frame
	//std::cout << ".";
	m_timer += dt;

	if (m_timer > 1.0 / m_animationSpeed){
		//std::cout << "POP!" <<  m_timer << " & "  << m_animationSpeed << "\n\n";
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

SpriteSheet::~SpriteSheet()
{
	//std::cout << "Destroying SpriteSheet " << this << " with texture " << m_texture << "\n";
	SDL_DestroyTexture(m_texture);
}