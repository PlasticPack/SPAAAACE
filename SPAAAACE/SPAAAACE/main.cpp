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



	Scene savesMenu("scripts/scene_menu", "saves/menu2.xml", "savesMenu");
	Scene menu("scripts/scene_menu", "saves/menu.xml", "menu");
	
	Scene save1("scripts/scene_game", "saves/save1.xml", "game");
	Scene save2("scripts/scene_game", "saves/save2.xml", "game");
	Scene save3("scripts/scene_game", "saves/save3.xml", "game");

	Message postman;

	postman.addMessage("main", "main", MS_SWITCHED, 1);
	std::shared_ptr<Scene> currentScene = std::make_shared<Scene>(menu);

	while (!quit)
	{

		currentScene->update(postman);

		savesMenu.syncTimers(*currentScene);
		menu.syncTimers(*currentScene);
		/*save1.syncTimers(*currentScene);
		save2.syncTimers(*currentScene);
		save3.syncTimers(*currentScene);*/

		if (postman.getMessage("Action", "Button", MS_EXIT_REQUEST)){
			quit = true;
		}
		else if (postman.getMessage("Action", "Button", MS_PLAY)){
			postman.addMessage("main", "main", MS_SWITCHED, 1);
			GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(savesMenu);
		}
		else if (postman.getMessage("Action", "Button", MS_SAVE1)){
			postman.addMessage("main", "main", MS_SWITCHED, 1);
			GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(save1);
		}
		else if (postman.getMessage("Action", "Button", MS_SAVE2)){
			postman.addMessage("main", "main", MS_SWITCHED, 1);
			GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(save2);
		}
		else if (postman.getMessage("Action", "Button", MS_SAVE3)){
			postman.addMessage("main", "main", MS_SWITCHED, 1);
			GraphicsSystem::reset();
			currentScene = std::make_shared<Scene>(save3);
		}

		if (postman.getMessage("Scene", "Menu", MS_RESETSAVE) == 1){
			save1.reset();
		}
		if (postman.getMessage("Scene", "Menu", MS_RESETSAVE) == 2){
			save2.reset();
		}
		if (postman.getMessage("Scene", "Menu", MS_RESETSAVE) == 3){
			save3.reset();
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