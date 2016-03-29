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

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) != 0){
		//log("SDL_Init error : " + SDL_GetError());
		return 1;
	}


	bool quit = false;
	SDL_Event e;

	//Scene menu("scripts/scene_menu");
	Scene game("scripts/scene_game", "game");
	Message postman;

	std::shared_ptr<Scene> currentScene = std::make_shared<Scene>(game);

	while (!quit)
	{

		currentScene->update(postman);

		if (postman.getMessage("Scene", "Input", MS_EXIT_REQUEST))
			quit = true;

		/*else if (postman.getMessage("menu", "Input", 0)){
			GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(menu);
		}
		else if (postman.getMessage("game", "Input", 0)) {
			GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(game);
		}*/

	}

	GraphicsSystem::close();

	SDL_Quit();

	return 0;
}