#include "InputHandler.h"
#include <iostream>

using namespace std;

InputsHandler::InputsHandler() : m_manette(NULL), m_haptic(NULL), m_inputClavier(NULL)
{
	m_nbAction = NUMBER_OF_ACTION - debutActionList;

	for (int i = debutActionList; i < NUMBER_OF_ACTION; i++)
	{
		m_action[i] = i - debutActionList;
	}

}

InputsHandler::~InputsHandler()
{
	closeJoystick();
}
//à venir
bool InputsHandler::loadConfig(string const& path)
{
	return true;
}

void InputsHandler::update()
{
	m_trigeredAction.clear();
	while (SDL_PollEvent(&m_event))
	{
		if (m_event.type == SDL_QUIT)
		{
			// PAr message
		}

		//Gestion de périphérique
		if (m_event.type == SDL_JOYDEVICEADDED)
		{
			openJoystick();
		}

		if (m_event.type == SDL_JOYDEVICEREMOVED)
		{
			closeJoystick();
		}

		// Inputs Joystick
		if (m_manette != NULL)
		{
			if (m_event.type == SDL_JOYAXISMOTION)
			{
				if (m_event.jaxis.which == 0)
				{
					if (m_event.jaxis.axis == 0)
					{
						//x joy gauche
					}
					else if (m_event.jaxis.axis == 1)
					{
						//y joy gauche
					}

				}


			}

			if (m_event.type == SDL_JOYBUTTONDOWN)
			{
				switch (m_event.jbutton.button)
				{
				case GP_BUTTON_A:
					cout << m_nbAction << endl;
					rumbleJoy(500);
					
					break;
				case GP_LEFT_STICK_PUSH:
					
					break;
				}

					
			}
			else if (m_event.type == SDL_JOYBUTTONUP)
			{
				switch (m_event.jbutton.button)
				{
				case GP_RB:
					cout << "bouton A lachée" << endl;
					
					break;
				}
			}
		}
	}
	//Inputs clavier
	const Uint8* inputClavier = SDL_GetKeyboardState(NULL);

	map<int, int > ::iterator it;
	for (it = m_action.begin(); it != m_action.end(); ++it)
	{
		if (inputClavier[it->second])
		{
			m_trigeredAction[it->first] = 1;
		}
	}
}

double InputsHandler::checkTriggeredAction(int const &flags)
{
	return m_trigeredAction[flags];
}

void InputsHandler::setActionTrigger(int const& actionFlag, int const& inputFlag)
{
	m_action[actionFlag] = inputFlag;
}

//JOYSTICk FUNCTION
bool InputsHandler::isJoyConnected()
{
	return m_manette != NULL;
}

void InputsHandler::rumbleJoy(unsigned temps, double puissance)
{
	if (SDL_HapticRumblePlay(m_haptic, puissance, temps) != 0)
	{
		cout << "unable to rumble!" << endl;
	}
}

bool InputsHandler::openJoystick()
{
	if (SDL_NumJoysticks() > 0)
	{
		closeJoystick();
		m_manette = SDL_JoystickOpen(0);
		if (m_manette == NULL)
			return false;
		cout << "nb. de Manette :" << SDL_NumJoysticks() << "\nID Manette :" << SDL_JoystickName(m_manette) << endl;
		m_haptic = SDL_HapticOpenFromJoystick(m_manette);
		if (m_haptic == NULL)
		{
			cout << "Rumble feature not activated: " <<SDL_GetError() << endl;
			return false;
		}
		if (SDL_HapticRumbleInit(m_haptic) < 0)
		{
			cout << "Rumble feature not activated" << endl;
			return false;
		}
		
		return true;
	}
	else
	{
		cout << "Aucun joystick détecté" << endl;
		return false;
	}
}

void InputsHandler::closeJoystick()
{
	SDL_HapticClose(m_haptic);
	m_haptic = NULL;
	SDL_JoystickClose(m_manette);
	m_manette = NULL;
}