#include "GraphicsSystem.h"


GraphicsSystem::GraphicsSystem()
{
	m_camera = { 0, 1, Vec2(SCREEN_W / 2, SCREEN_H / 2) , true};
	m_defaultHalo = NULL;
	m_countedFrames = 1;
	m_avgFPS = 60;
	for (int i = 0; i < 4; i++)
		m_backgrounds[i] = nullptr;
	m_frameStarted = false;
	IMG_Init(IMG_INIT_PNG);
	m_window = SDL_CreateWindow("Physics - SPACE SIM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	if (m_window == nullptr){
		//log("SDL_CreateWindow error : " + string(SDL_GetError()));
		SDL_Quit();
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED );
	if (m_renderer == nullptr){
		SDL_DestroyWindow(m_window);
		//log("SDL_CreateRenderer error : " + string(SDL_GetError()));
		SDL_Quit();
	}

	m_defaultHalo = this->loadTexture("ressources/halo.png");
}

double GraphicsSystem::getFPS(){
	return m_avgFPS;
}

void GraphicsSystem::lockCamera(bool l){
	m_camera.locked = l;
}
GraphicsSystem::~GraphicsSystem()
{
	SDL_DestroyTexture(m_backgrounds[0]);
	SDL_DestroyTexture(m_backgrounds[1]);
	SDL_DestroyTexture(m_backgrounds[2]);
	SDL_DestroyTexture(m_backgrounds[3]);
	SDL_DestroyTexture(m_defaultHalo);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	IMG_Quit();
}


SDL_Texture* GraphicsSystem::loadTexture(const std::string filename, Uint8 r, Uint8 g, Uint8 b){

	SDL_Texture* texture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(filename.c_str());
	if (loadedSurface == NULL){
		//error loaded
		std::cout << "COULD NOT LOAD SURFACE FROM " << filename << " " << SDL_GetError() << "\n";
	}
	else {
		SDL_SetColorKey(loadedSurface,SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0,0,0, 255));
		SDL_SetSurfaceColorMod(loadedSurface, r, g, b);
		texture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
	}

	//std::cout << "ADRESS OF TEXTURE IN LOADIN : " << texture << "\n";

	return texture;
}

void GraphicsSystem::setCameraTarget(Vec2 t){
	m_camera.target = t;
}

void GraphicsSystem::setCameraZoom(double zoom){
	m_camera.zoom = zoom;
}

void GraphicsSystem::setCameraAngle(double angle){
	m_camera.angle = angle;
}

void GraphicsSystem::setCamera(Vec2 t, double z, double a){
	m_camera = { a, z, t };
}

void GraphicsSystem::loadBackground(const std::string filename, int layer, Uint8 r, Uint8 g, Uint8 b){
	if (layer < 4 && layer >= 0){
		if(m_backgrounds[layer] != NULL)
			SDL_DestroyTexture(m_backgrounds[layer]);

		m_backgrounds[layer] = loadTexture(filename, r, g, b);
		int w(0), h(0);
		SDL_QueryTexture(m_backgrounds[layer], NULL, NULL, &w, &h);
		m_backgroundSize[layer] = Vec2(w, h);
	}
}

void GraphicsSystem::update(GraphicsComponent gComp, double dt){
	if (m_frameStarted){

		if (gComp.getSprite() != nullptr) {
			gComp.getSprite()->getCurrentSpriteSheet()->nextRect(dt);

			Vec2 coord, tempPos, finalPos, newPos, screenCoord(SCREEN_W, SCREEN_H);
			coord = gComp.getPosition();
			newPos = coord;

			SDL_Rect sprRect = gComp.getSprite()->getCurrentSpriteSheet()->getCurrentRect();
			double zIndex = (gComp.getPositionComponent()->getZIndex());
			//zIndex *= zIndex;
			double zoom(1), angle(0);
			
			if (m_camera.locked){
				zoom = m_camera.zoom;
				angle = m_camera.angle;
				tempPos = coord - m_camera.target;

				double newX = zIndex * m_camera.zoom * ((cos(m_camera.angle *(3.14159265 / 180)) * tempPos.x()) + (sin(m_camera.angle *(3.14159265 / 180)) * tempPos.y()));
				double newY = zIndex * m_camera.zoom * ((-sin(m_camera.angle *(3.14159265 / 180)) * tempPos.x()) + (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.y()));

				newPos = Vec2(newX, newY);

				newPos += (screenCoord / 2);
			}

			
			SDL_Rect pos = { newPos.x() - (gComp.getSize().x() * zIndex * m_camera.zoom / 2), newPos.y() - (zIndex * m_camera.zoom * gComp.getSize().y() / 2), gComp.getSize().x() * zIndex * zoom, gComp.getSize().y() * zIndex * zoom };

			//std::cout << "ADRESS OF TEXTURE IN UPDATE : " << gComp.getSprite()->getCurrentSpriteSheet()->getTexture() << "\n";

			/*if (gComp.isHaloOn()){
				float sizeX(0), sizeY(0);
				sizeX = zIndex * zoom * sprRect.w;
				sizeY = zIndex * zoom * sprRect.h;
				float deltaX, deltaY;
				deltaX = (4 * sin(SDL_GetTicks() / 430.0) + 5) * zIndex * zoom;
				deltaY = (4 * sin(SDL_GetTicks() / 430.0) + 5) * zIndex * zoom;
				SDL_Rect haloRect = { newPos.x() - ((sizeX/2) + deltaX) , newPos.y() - ((sizeY/2) + deltaX), 2 * (sizeX + deltaX), 2 * (sizeY+deltaY) };
				//std::cout << gComp.getHaloColor().r << "\n";
				SDL_SetTextureColorMod(m_defaultHalo, (unsigned int)gComp.getHaloColor().r, (unsigned int)gComp.getHaloColor().g, (unsigned int)gComp.getHaloColor().b);
				SDL_RenderCopyEx(m_renderer, m_defaultHalo, NULL, &haloRect, angle, NULL, SDL_FLIP_NONE);
			}*/

			SDL_RenderCopyEx(m_renderer, gComp.getSprite()->getCurrentSpriteSheet()->getTexture(), &sprRect, &pos, angle, NULL,SDL_FLIP_NONE);
			
		}
	}
}

void GraphicsSystem::initFrame(){
	if (!m_frameStarted)
	{
		if (!m_fpsTimer.isStarted())
			m_fpsTimer.start();

		m_avgFPS = m_countedFrames / (1 + m_fpsTimer.getTicks() / 1000.0);
		//std::cout << m_countedFrames << " / " << m_fpsTimer.getTicks() / 1000.0 <<  " = "  << m_avgFPS << "<- :D\n";
		if (m_avgFPS > 20000 ){
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

				if (m_camera.locked){
					t += m_camera.target *-1;

					//double newX = (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.x) + (sin(m_camera.angle *(3.14159265 / 180)) * tempPos.y);
					//double newY = (-sin(m_camera.angle *(3.14159265 / 180)) * tempPos.x) + (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.y);

					t *= ((i + 1) * 0.15) * m_camera.zoom; // *newX;

					t += screenCoord / 2;

					tempPos.x = t.x();
					tempPos.y = t.y();

					pos.x = tempPos.x % (int)bgSize.x();
					pos.y = tempPos.y % (int)bgSize.y();
				}
				int totalX = m_camera.zoom * SCREEN_W / bgSize.x();
				totalX += 2;

				int totalY = m_camera.zoom * SCREEN_H / bgSize.y();
				totalY += 2;

				for (int x = (-totalX / 2 ) - 1; x <= (totalX / 2 )+ 1; x++){
					tempPos.x = pos.x + (x * bgSize.x());
					for (int y = (-totalY / 2) - 1; y <= (totalY / 2) + 1; y++){
						tempPos.y = pos.y + (y * bgSize.y());
						//SDL_RenderCopyEx(m_renderer, m_backgrounds[i], NULL, &tempPos, m_camera.angle, NULL, SDL_FLIP_NONE);
						SDL_RenderCopy(m_renderer, m_backgrounds[i], NULL, &tempPos);
					}
				}

				/*SDL_Rect posRect = { 0, 0, SCREEN_W, SCREEN_H, };
				double zoom(1), angle(0), newX, newY;
				Vec2 tempPos = Vec2(0, 0);

				double posConstant = 1;

				if (m_camera.locked){
				zoom = m_camera.zoom;
				angle = m_camera.angle;
				tempPos = m_camera.target * -1;
				posConstant = ((i + 1) * 0.2) * zoom;

				newX = posConstant * ((cos(m_camera.angle *(3.14159265 / 180)) * tempPos.x()) + (sin(m_camera.angle *(3.14159265 / 180)) * tempPos.y()));
				newY = posConstant * ((-sin(m_camera.angle *(3.14159265 / 180)) * tempPos.x()) + (cos(m_camera.angle *(3.14159265 / 180)) * tempPos.y()));

				posRect.x = (int)newX % (int)(SCREEN_W * posConstant);
				posRect.y = (int)newY % (int)(SCREEN_H * posConstant);

				//posRect.w *= posConstant;
				//posRect.h *= posConstant;
				}

				SDL_Rect tempRect = posRect;

				int total = ceil(1 / posConstant) + 1;
				for (int x = ((total - 1) / -2) -1; x < ((total - 1) / 2) + 2; x++){
				//std::cout << x <<" \n";
				tempRect.x = posRect.x + (x * SCREEN_W * zoom);//- (tempPos.x() * zoom * ((i + 0.1) * 1.2));
				for (int y = ((total - 1) / -2) - 1; y < ((total - 1) / 2) + 2; y++){
				tempRect.y = posRect.y + (y * SCREEN_H * zoom); // -(tempPos.y() * zoom * ((i + 0.1) * 1.2));
				SDL_RenderCopyEx(m_renderer, m_backgrounds[i], NULL, &tempRect, m_camera.angle, NULL, SDL_FLIP_NONE);
				}
				}*/
			}
		}
		m_frameStarted = true;
	}
}

void GraphicsSystem::endFrame(){
	if (m_frameStarted){
		//std::cout << "RENDERING\n";
		SDL_RenderPresent(m_renderer);
		m_frameStarted = false;
		m_countedFrames++;
	}
}