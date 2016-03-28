#include "GraphicsSystem.h"

SDL_Window* GraphicsSystem::m_window = NULL;//SDL_CreateWindow("Physics - SPACE SIM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
SDL_Renderer* GraphicsSystem::m_renderer = NULL;//SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
TTF_Font* GraphicsSystem::m_currentFont = NULL;
Camera GraphicsSystem::m_camera = { 0, 1, Vec2(SCREEN_W / 2, SCREEN_H / 2), true };
bool GraphicsSystem::m_frameStarted = false;
double GraphicsSystem::m_avgFPS = 60;
LTimer GraphicsSystem::m_fpsTimer = LTimer();
int GraphicsSystem::m_countedFrames = 1;
SDL_Texture* GraphicsSystem::m_backgrounds[4];
Vec2 GraphicsSystem::m_backgroundSize[4];
bool GraphicsSystem::m_initialized = false;
SDL_Texture * GraphicsSystem::m_currentTextTexture = NULL;
SDL_Color GraphicsSystem::m_textColor = { 255, 255, 255 };
std::string GraphicsSystem::m_currentText = "";

GraphicsSystem::GraphicsSystem()
{
	
}

void GraphicsSystem::init(){
	if (!m_initialized){
		std::cout << "Initializing visuals \n";
		for (int i = 0; i < 4; i++)
			m_backgrounds[i] = nullptr;

		IMG_Init(IMG_INIT_PNG);
		m_window = SDL_CreateWindow("Physics - SPACE SIM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
		if (m_window == NULL){
			std::cout << "SDL_CreateWindow error : " + std::string(SDL_GetError());
			SDL_Quit();
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
		if (m_renderer == NULL){
			SDL_DestroyWindow(m_window);
			std::cout << "SDL_CreateRenderer error : " + std::string(SDL_GetError());
			SDL_Quit();
		}

		TTF_Init();

		m_initialized = true;
	}
}

double GraphicsSystem::getFPS(){
	return m_avgFPS;
}

void GraphicsSystem::lockCamera(bool l){
	m_camera.locked = l;
}

GraphicsSystem::~GraphicsSystem()
{
}



void GraphicsSystem::close(){
	if (m_initialized){
		std::cout << "\n\nDEST";
		SDL_DestroyTexture(m_backgrounds[0]);
		SDL_DestroyTexture(m_backgrounds[1]);
		SDL_DestroyTexture(m_backgrounds[2]);
		SDL_DestroyTexture(m_backgrounds[3]);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		TTF_CloseFont(m_currentFont);
		IMG_Quit();
		TTF_Quit();
	}
}

void GraphicsSystem::setFont(std::string f, int s, SDL_Color c){
	if (m_currentFont != NULL)
		TTF_CloseFont(m_currentFont);
	m_currentFont = TTF_OpenFont(f.c_str(), s);

	m_textColor = c;
}

void GraphicsSystem::setTextColor(SDL_Color c){
	m_textColor = c;
}

void GraphicsSystem::print(std::string f){

	if (f != m_currentText){

		m_currentText = f;

		if (m_currentTextTexture != NULL){
			SDL_DestroyTexture(m_currentTextTexture);
		}

		SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(m_currentFont, f.c_str(), m_textColor, SCREEN_W - 60);
		m_currentTextTexture = SDL_CreateTextureFromSurface(m_renderer, surf);

		SDL_FreeSurface(surf);
	}

	if (m_currentTextTexture != NULL){

		SDL_Rect maxRect = { 15, SCREEN_H - 224 - 45, SCREEN_W - 30, 224+30};

		SDL_Rect msgRect = maxRect;

		int w(0), h(0);


		msgRect.x += 15;
		msgRect.w -= 30;
		msgRect.h -= 30;
		msgRect.y += 15;


		TTF_SizeText(m_currentFont, m_currentText.c_str(), &w, &h); 

		msgRect.h = h * ceil((double)w / (double)(msgRect.w));
		if (ceil((double)w / (double)(msgRect.w)) == 1){
			msgRect.w = w;
		}

		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(m_renderer, 50, 50, 65, 125);
		SDL_RenderFillRect(m_renderer, &maxRect);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderCopy(m_renderer, m_currentTextTexture, NULL, &msgRect);
	}

}

SDL_Texture* GraphicsSystem::loadTexture(const std::string filename, Uint8 r, Uint8 g, Uint8 b){

	if (m_initialized) {
		SDL_Texture* texture = NULL;

		SDL_Surface *loadedSurface = IMG_Load(filename.c_str());
		if (loadedSurface == NULL){
			//error loaded
			std::cout << "COULD NOT LOAD SURFACE FROM " << filename << " " << SDL_GetError() << "\n";
		}
		else {
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0, 0, 0, 255));
			SDL_SetSurfaceColorMod(loadedSurface, r, g, b);
			texture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
			SDL_FreeSurface(loadedSurface);
		}

		//std::cout << "ADRESS OF TEXTURE IN LOADIN : " << texture << "\n";

		return texture;
	}
}

void GraphicsSystem::setCameraTarget(Vec2 t){
	m_camera.target = t;
}

void GraphicsSystem::setCameraZoom(double zoom){
	if (zoom > 0)
		m_camera.zoom = zoom;
}

void GraphicsSystem::setCameraAngle(double angle){
	m_camera.angle = angle;
}

void GraphicsSystem::setCamera(Vec2 t, double z, double a){
	m_camera = { a, z, t };
}

void GraphicsSystem::reset(){
	if (m_initialized){
		std::cout << "Reset\n";
		m_camera = { 0, 1, Vec2(SCREEN_W / 2, SCREEN_H / 2), true };
	}
}

void GraphicsSystem::loadBackground(const std::string filename, int layer, Uint8 r, Uint8 g, Uint8 b){
	if (m_initialized){
		if (layer < 4 && layer >= 0){
			if (m_backgrounds[layer] != NULL)
				SDL_DestroyTexture(m_backgrounds[layer]);

			m_backgrounds[layer] = loadTexture(filename, r, g, b);
			int w(0), h(0);
			SDL_QueryTexture(m_backgrounds[layer], NULL, NULL, &w, &h);
			m_backgroundSize[layer] = Vec2(w, h);
		}
	}
}

void GraphicsSystem::update(Message &postman, GraphicsComponent gComp, double dt) {
	if (m_initialized) {
		if (m_frameStarted){


			//si on recoit qu'il y a eu collision
			//std::cout << postman.getMessage("Physics", "Physics", MS_COLLISION) << "sadsdsad\n";
			if (postman.getMessage("GameLogic", "GameLogic", MS_COLLISION) > 1000.0){
				gComp.getSprite()->setSpriteSheet("collision");
			}

			if (gComp.getSprite() != nullptr) {
				gComp.getSprite()->getCurrentSpriteSheet()->nextRect(dt);

				SDL_Rect sprRect = gComp.getSprite()->getCurrentSpriteSheet()->getCurrentRect();
				if (gComp.isAffectedByCamera()){

					Vec2 coord, tempPos, finalPos, newPos, screenCoord(SCREEN_W, SCREEN_H);
					coord = gComp.getPosition();
					newPos = coord;

					double zIndex = (gComp.getPositionComponent()->getZIndex());
					//zIndex *= zIndex;
					double zoom(1), angle(0);

					zoom = m_camera.zoom;
					angle = m_camera.angle;

					tempPos = coord - (screenCoord / 2);
					//std::cout << tempPos.x() << "\n";

					if (m_camera.locked){
						tempPos = coord - m_camera.target;
					}

					double newX = zIndex * m_camera.zoom * ((cos(m_camera.angle *(3.14159265 / 180)) * tempPos.x()) + (sin(m_camera.angle *(3.14159265 / 180)) * tempPos.y()));
					double newY = zIndex * m_camera.zoom * ((-sin(m_camera.angle *(3.14159265 / 180)) * tempPos.x()) + (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.y()));
					newPos = Vec2(newX, newY);

					//newPos *= zIndex * m_camera.zoom;
					newPos += (screenCoord / 2);

					SDL_Rect pos = { newPos.x() - (gComp.getSize().x() * zIndex * zoom / 2), newPos.y() - (zIndex * zoom * gComp.getSize().y() / 2), gComp.getSize().x() * zIndex * zoom, gComp.getSize().y() * zIndex * zoom };

					//SDL_Rect pos2 = { 10, 10, 100, 100 };
					//std::cout << "ADRESS OF TEXTURE IN UPDATE : " << gComp.getSprite()->getCurrentSpriteSheet()->getTexture() << "\n";

					SDL_RenderCopyEx(m_renderer, gComp.getSprite()->getCurrentSpriteSheet()->getTexture(), &sprRect, &pos, angle, NULL, SDL_FLIP_NONE);

				}
				else {
					SDL_Rect pos = { gComp.getPosition().x(), gComp.getPosition().y(), gComp.getSize().x(), gComp.getSize().y() };
					SDL_RenderCopy(m_renderer, gComp.getSprite()->getCurrentSpriteSheet()->getTexture(), &sprRect, &pos);
				}
			}
		}
	}
}

void GraphicsSystem::initFrame(){
	if (m_initialized){
		if (!m_frameStarted)
		{
			if (!m_fpsTimer.isStarted())
				m_fpsTimer.start();

			m_avgFPS = m_countedFrames / (1 + m_fpsTimer.getTicks() / 1000.0);
			//std::cout << m_countedFrames << " / " << m_fpsTimer.getTicks() / 1000.0 <<  " = "  << m_avgFPS << "<- :D\n";
			if (m_avgFPS > 200000){
				m_avgFPS = 60;
			}

			//SDL_SetRenderDrawColor(m_renderer, 12, 0, 24, 0);
			SDL_RenderClear(m_renderer);
			//repeating background

			for (int i = 0; i < 4; i++){
				if (m_backgrounds[i] != NULL){

					Vec2 bgSize(m_backgroundSize[i]);
					SDL_Rect pos = { 0, 0, bgSize.x(), bgSize.y() };
					SDL_Rect tempPos = pos;
					Vec2 t = Vec2(pos.x, pos.y);
					Vec2 screenCoord(SCREEN_W, SCREEN_H);
					SDL_Point center = { SCREEN_W / 2, SCREEN_H / 2 };

					if (m_camera.locked){
						t += m_camera.target *-1;

						//double newX = (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.x) + (sin(m_camera.angle *(3.14159265 / 180)) * tempPos.y);
						//double newY = (-sin(m_camera.angle *(3.14159265 / 180)) * tempPos.x) + (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.y);
					}

					t *= (((i*i*i) + 1) * 0.075); // *newX;
					t *= m_camera.zoom;

					t += screenCoord / 2;

					tempPos.x = t.x();
					tempPos.y = t.y();

					pos.x = tempPos.x % (int)(bgSize.x() * m_camera.zoom);
					pos.y = tempPos.y % (int)(bgSize.y() * m_camera.zoom);

					tempPos.w = bgSize.x() * m_camera.zoom;
					tempPos.h = bgSize.y() * m_camera.zoom;


					//SDL_RenderCopyEx(m_renderer, m_backgrounds[i], NULL, &tempPos, m_camera.angle, NULL, SDL_FLIP_NONE);

					int totalX = floor(1 + (SCREEN_W / (bgSize.x() * m_camera.zoom)));
					//totalX += 2;

					int totalY = floor(1 + (SCREEN_H / (bgSize.y() * m_camera.zoom)));
					//totalY += 2;

					for (int x = (-totalX / 2) - 1; x <= (totalX / 2) + 1; x++){
						tempPos.x = pos.x + (x * bgSize.x() * m_camera.zoom);
						for (int y = (-totalY / 2) - 1; y <= (totalY / 2) + 1; y++){
							tempPos.y = pos.y + (y * bgSize.y() * m_camera.zoom);
							SDL_RenderCopyEx(m_renderer, m_backgrounds[i], NULL, &tempPos, m_camera.angle, &center, SDL_FLIP_NONE);
							//SDL_RenderCopyEx(m_renderer, m_backgrounds[i], NULL, &tempPos, );
						}
					}
				}
			}
			m_frameStarted = true;
		}
	}
}

void GraphicsSystem::endFrame(){
	if (m_initialized) {
		if (m_frameStarted){
			//std::cout << "RENDERING\n";
			SDL_RenderPresent(m_renderer);
			m_frameStarted = false;
			m_countedFrames++;
		}
	}
}