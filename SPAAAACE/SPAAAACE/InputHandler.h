#pragma once
#include <SDL.h>
#include <string>


enum 
{
	GP_DPAD_UP,
	GP_DPAD_DOWN,
	GP_DPAD_LEFT,
	GP_DPAD_RIGHT,
	GP_SELECT,
	GP_START,
	GP_LEFT_STICK_PUSH,
	GP_RIGHT_STICK_PUSH,
	GP_LB,
	GP_RB,
	GP_BUTTON_A,
	GP_BUTTON_B,
	GP_BUTTON_X,
	GP_BUTTON_Y
};

enum
{
	AC_START = 0x9000
};

struct Action
{
	int inputFlag;
	int actionFlag;
};

class InputsHandler
{
private:
	
	Uint8* m_inputClavier;
	SDL_Event m_event;
	SDL_Joystick* m_manette;

	

	bool openJoystick();
	void closeJoystick();

public:
	//Initialise la manette 0 s'il en a une de connecté
	InputsHandler();

	~InputsHandler();

	/*Permet de loader une configuration de touche *PAS FAIT*
	*@string path : chemin vers le fichier contenant les directives
	²@return bool : vraie si tout ce passe bien, faux s'il y a une erreur.
	*/
	bool loadConfig(std::string const& path);
	/*
	*Prends tout les inputs
	*
	*/
	void update(); 
};