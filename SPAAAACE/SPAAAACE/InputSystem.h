/*
Auteur : Gabriel Bouchard
Buts: Syst�me qui g�re les inputs
*/
#pragma once
#include <SDL.h>
#include <string>
#include <map>
#include "Flags.h"
#include "Vec2.h"
#include <math.h>


//class InputsComponent;

class InputsSystem
{
private:
	
	
	Uint8* m_inputClavier;
	SDL_Event m_event;

	//variable pour manettes
	SDL_Joystick* m_manette;
	SDL_Haptic* m_haptic;
	unsigned m_deadzone;
	
	std::multimap<int,std::pair<int,double> > m_action;
	
	//fonction priv�e
	bool openJoystick();
	void closeJoystick();
	void clearTriggeredAction();

public:
	//Initialise les actions
	InputsSystem();

	~InputsSystem();

	/* *PAS FAIT* Permet de loader une configuration de touche *PAS FAIT*
	*@string path : chemin vers le fichier contenant les directives
	�@return bool : vraie si tout ce passe bien, faux s'il y a une erreur.
	*/
	bool loadConfig(std::string const& path);
	/*
	*Prends tout les inputs
	*!!!S'ASSURER QUE CETTE FONCTION SOIT UPDAT� TOUS LES FRAMES POUR UN BON FONCITONNEMENT!!!!
	* |->checkTriggeredAction()<-| pour voir le statut des actions
	*/
	void pollInputs();

	//void update(InputsComponent* inComp);

	/*
	* Renvoi le statut de l'action demend�
	*@param actionFlag : l'action que vous voulez regarder
	* Les actions commencent par *AC_*
	*ex de actionFlag : AC_EXIT pour voir si l'utilisateur veut quitter
	* encodage de valeur : 0 = false, reste = true 
	*si valeur analogique (joystick) renvoie la valeur en tant que tel
	*/
	double checkTriggeredAction(int const &actionFlags);

	/*
	*Permet de connecter une action � un input
	*Les actions commencent par "AC_"
	*ex de actionFlag : AC_EXIT
	*Les inputs au clavier commencent par "SDL_SCANCODE_"
	*Les inputs � la manette commencent par "GP_"
	*ex de inputFlag : SDL_SCANCODE_UP pour la fl�che en haut
	*/
	void setActionTrigger(int const& actionFlag, int const& inputFlag);

	void eraseInputsInAction(int const& actionFlag);

	/*
	*Retourne si le joystick est connect�
	*/
	bool isJoyConnected();
	void rumbleJoy(unsigned temps, double puissance = 1);

	void setJoyDeadzone(unsigned const& deadzone);
	unsigned getJoyDeadzone();

};