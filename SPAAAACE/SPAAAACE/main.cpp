#include <SDL.h>
#include <iostream>
#include <math.h>
#include "InputHandler.h"
#include <map>
using namespace std;
#undef main
int main(int argc, char* args[])
{
	bool run = true;
	const int WIDTH = 1000;
	const int HEIGHT = 600;

	//Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* fenetre;
	SDL_Renderer* rendeur;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &fenetre, &rendeur);
	
	InputsHandler in;

	in.setActionTrigger(AC_UP, SDL_SCANCODE_UP);
	while (run)
	{
		in.update();
		if (in.checkTriggeredAction(AC_UP))
			cout << "UP!!" << endl;
	}

	//Quit SDL
	SDL_Quit();

	

	return 0;
}

