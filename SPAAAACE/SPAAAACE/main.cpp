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
	in.setActionTrigger(AC_PRINCIPAL_KEY, GP_BUTTON_A);
	in.setActionTrigger(AC_SECONDARY_KEY, GP_BUTTON_B);
	in.setActionTrigger(AC_EXIT, SDL_SCANCODE_ESCAPE);
	in.setActionTrigger(AC_VERTICAL_PUSH, GP_AXIS_LEFT_JOY_Y);
	in.setActionTrigger(AC_HORIZONTAL_PUSH, GP_AXIS_LEFT_JOY_X);
	while (run)
	{
		in.update();
		if (in.checkTriggeredAction(AC_UP))
			cout << "UP!!" << endl;
		if (in.checkTriggeredAction(AC_PRINCIPAL_KEY))
			cout << "A" << endl;
		if (in.checkTriggeredAction(AC_SECONDARY_KEY))
			cout << "B" << endl;
		int pushY = in.checkTriggeredAction(AC_VERTICAL_PUSH);
		int pushX = in.checkTriggeredAction(AC_HORIZONTAL_PUSH);
		cout << "("<< pushX << "," <<pushY << ")"<< endl;
		if (in.checkTriggeredAction(AC_EXIT))
			run = false;
	}

	//Quit SDL
	SDL_Quit();

	

	return 0;
}

