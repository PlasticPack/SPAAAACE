#include "GraphicsSystem.h"


GraphicsSystem::GraphicsSystem()
{
	m_frameStarted = false;

	m_window = SDL_CreateWindow("Physics - SPACE SIM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1100, 700, SDL_WINDOW_SHOWN);
	if (m_window == nullptr){
		//log("SDL_CreateWindow error : " + string(SDL_GetError()));
		SDL_Quit();
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr){
		SDL_DestroyWindow(m_window);
		//log("SDL_CreateRenderer error : " + string(SDL_GetError()));
		SDL_Quit();
	}
}


GraphicsSystem::~GraphicsSystem()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
}


SDL_Texture* GraphicsSystem::loadTexture(const std::string filename){

	SDL_Texture* texture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(filename.c_str());
	if (loadedSurface == NULL){
		//error loaded
		std::cout << "COULD NOT LOAD SURFACE FROM " << filename << " " << SDL_GetError() << "\n";
	}
	else {
		texture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
	}

	std::cout << "ADRESS OF TEXTURE IN LOADIN : " << texture << "\n";

	return texture;
}

void GraphicsSystem::update(GraphicsComponent gComp, double dt){
	if (m_frameStarted){

		//std::cout << "\nFIRST\n";
		gComp.getSprite()->getCurrentSpriteSheet()->nextRect(dt);
		SDL_Rect sprRect = gComp.getSprite()->getCurrentSpriteSheet()->getCurrentRect();
		double zIndex = gComp.getPositionComponent()->getZIndex();
		
		SDL_Rect pos = { gComp.getPosition().x(), gComp.getPosition().y(), sprRect.w * zIndex, sprRect.h * zIndex };
		//SDL_Texture* texture = loadTexture("test.png");

		//std::cout << "ADRESS OF TEXTURE IN UPDATE : " << gComp.getSprite()->getCurrentSpriteSheet()->getTexture() << "\n";
		SDL_RenderCopy(m_renderer, gComp.getSprite()->getCurrentSpriteSheet()->getTexture(), &sprRect, &pos);

		//std::cout << "LAST\n";
		//}
	}
}

void GraphicsSystem::initFrame(){
	if (!m_frameStarted)
	{
		SDL_RenderClear(m_renderer);
		//affichage background
		//rect noir pour là
		//SDL_SetRenderDrawColor(m_renderer, 0, 0, 6, 0);
		//SDL_Rect fillRect = { 0, 0, 1100, 700 };
		//SDL_RenderFillRect(m_renderer, &fillRect);
		
		
		m_frameStarted = true;
	}
}

void GraphicsSystem::endFrame(){
	if (m_frameStarted){
		//std::cout << "RENDERING\n";
		SDL_RenderPresent(m_renderer);
		m_frameStarted = false;
	}
}