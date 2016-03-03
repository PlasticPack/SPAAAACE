#include <iostream>
#include <SDL.h>
#include "Scene.h"
#include "Message.h"
const int SCREEN_W = 1600;
const int SCREEN_H = 900;

#undef main

int main(int argc, char* argv[]){

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
		//log("SDL_Init error : " + SDL_GetError());
		return 1;
	}


	bool quit = false;
	SDL_Event e;

	Scene mainScene;
	Message postman;

	while (!quit)
	{

		mainScene.update(postman);
		if (postman.getMessage("Scene", "Input", MS_EXIT_REQUEST))
			quit = true;
		/*Vec2 pos1 = mainScene.getObjects()[0].get<PhysicsComponent>()->getPosition();
		Vec2 pos2 = mainScene.getObjects()[1].get<PhysicsComponent>()->getPosition();
		Vec2 pos3 = mainScene.getObjects()[2].get<PhysicsComponent>()->getPosition();
		Vec2 pos4 = mainScene.getObjects()[3].get<PhysicsComponent>()->getPosition();*/


		/*SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_Rect fillRect = { 0, 0, SCREEN_W, SCREEN_H };
		SDL_RenderFillRect(renderer, &fillRect);*/

		/*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(renderer, pos1.x(), pos1.y());
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawPoint(renderer, pos2.x(), pos2.y());

		SDL_SetRenderDrawColor(renderer, 100, 205, 0, 255);
		SDL_RenderDrawPoint(renderer, pos3.x(), pos3.y());

		SDL_SetRenderDrawColor(renderer, 200, 255, 0, 255);
		SDL_RenderDrawPoint(renderer, pos4.x(), pos4.y());*/

		//SDL_RenderPresent(renderer);
	}

	/*SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);*/
	SDL_Quit();

	return 0;
}