#include "main_Graphics.h"
#include <SDL_image.h>

namespace Graphics {

	SDL_Texture* loadTexture(const std::string filename, SDL_Renderer *ren){
		SDL_Texture *texture = NULL;

		SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
		if (loadedSurface == NULL){
			std::cout << "ERROR LOADED SUR\n";
		}
		else {
			texture = SDL_CreateTextureFromSurface(ren, loadedSurface);
			if (texture == NULL){
				//error
			}
			SDL_FreeSurface(loadedSurface);
		}

		return texture;
	}

	void drawSpriteSheet(SpriteSheet &sprSht, double dt, SDL_Renderer *ren, Vec2 coord, Camera cam){

		coord.setCoords(coord.x() + 45, coord.y() + 45);
		Vec2 tempPos, finalPos, screenCoord(SCREEN_W, SCREEN_H);
		
		tempPos = coord - cam.target;

		//tempPos += (screenCoord / (2 * cam.zoom));

		double newX = cam.zoom * ((cos(cam.angle *(3.14159265 / 180)) * tempPos.x()) + (sin(cam.angle *(3.14159265 / 180)) * tempPos.y()));
		double newY = cam.zoom * ((-sin(cam.angle *(3.14159265 / 180)) * tempPos.x()) + (cos(cam.angle *(3.14159265 / 180)) * tempPos.y()));

		Vec2 newVec(newX, newY);

		newVec += (screenCoord / 2);

		/*Vec2 relativeToTarget(coord - cam.target);
		//std::cout << (-relativeToTarget.getNormalized().y()) << "\n";

		if (relativeToTarget.getLength() > 0){
			double initialAngle = (asin(relativeToTarget.getNormalized().x()) * 180 / 3.14159265) + 90;
			std::cout << initialAngle << "\n";
			Vec2 unitRelative = relativeToTarget.getNormalized();
		}*/

		sprSht.nextRect(dt);
		SDL_Rect sprRect = sprSht.getCurrentRect();
		SDL_Rect pos { newVec.x(), newVec.y(), sprRect.w * cam.zoom, sprRect.h * cam.zoom};
		SDL_Point centre;
		centre.x = sprRect.w / 2.0;
		centre.y = sprRect.h / 2.0;
		SDL_RenderCopyEx(ren, sprSht.getTexture(), &sprRect, &pos, cam.angle, &centre , SDL_FLIP_NONE);
	}

}