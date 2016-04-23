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



	//Scene menu("scripts/scene_menu", "menu");



	Scene game("scripts/scene_game", "game");
	Message postman;

	postman.addMessage("main", "main", MS_SWITCHED, 1);
	std::shared_ptr<Scene> currentScene = std::make_shared<Scene>(game);

	while (!quit)
	{

		currentScene->update(postman);

		if (postman.getMessage("Action", "Button", MS_EXIT_REQUEST)){
			quit = true;
		}
		else if (postman.getMessage("Action", "Button", MS_PLAY)){
			postman.addMessage("main", "main", MS_SWITCHED, 1);
			currentScene = std::make_shared<Scene>(game);
		}
		/*else if (postman.getMessage("Action", "Button", MS_MENU)) {
			//GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(menu);
		}*/

	}
	GraphicsSystem::close();

	SDL_Quit();

	return 0;
}