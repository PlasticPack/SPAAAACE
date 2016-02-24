#include <SDL.h>
#include <iostream>
#include <math.h>
#include "InputHandler.h"
using namespace std;
#undef main
int main(int argc, char* args[])
{
	bool run = true;
	const int WIDTH = 1000;
	const int HEIGHT = 600;
	
	int jx = 0;
	int jy = 0;
	//Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* fenetre;
	SDL_Renderer* rendeur;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &fenetre, &rendeur);
	SDL_Joystick *manette = NULL;
	
	
	InputsHandler in;
	while (run)
	{
		in.update();
		if (jx !=0 || jy != 0)
		cout << "(" << jx << "," << jy << ")" << endl;

		const Uint8* inputClavier = SDL_GetKeyboardState(NULL);
		if (inputClavier[SDL_SCANCODE_UP])
		{
			cout << "up" << endl;
		}

		
	}



	

	//Quit SDL
	SDL_Quit();

	return 0;
}