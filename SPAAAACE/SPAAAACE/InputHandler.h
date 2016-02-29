#pragma once
#include <SDL.h>
#include <string>
#include <map>
#include "Flags.h"
#include "Vec2.h"
#include <math.h>


class InputsHandler
{
private:
	

	Uint8* m_inputClavier;
	SDL_Event m_event;

	//variable pour manettes
	SDL_Joystick* m_manette;
	SDL_Haptic* m_haptic;
	unsigned m_deadzone;
	
	std::map<int,int> m_action;
	std::map<int, double> m_trigeredAction;
	
	//fonction privée
	bool openJoystick();
	void closeJoystick();
	void clearTriggeredAction();

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

	double checkTriggeredAction(int const &flags);
	void setActionTrigger(int const& actionFlag, int const& inputFlag);



	bool isJoyConnected();
	void rumbleJoy(unsigned temps, double puissance = 1);


};