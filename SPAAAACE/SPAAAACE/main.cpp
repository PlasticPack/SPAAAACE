#include <iostream>
#include <SDL.h>
#include "Scene.h"
/*
=======
#include "Message.h"
>>>>>>> master
const int SCREEN_W = 1600;
const int SCREEN_H = 900;*/



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

	}

	SDL_Quit();

	return 0;
}