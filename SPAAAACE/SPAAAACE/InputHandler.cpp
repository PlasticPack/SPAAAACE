#include "InputHandler.h"
#include <iostream>

using namespace std;

InputsHandler::InputsHandler()
{
}

InputsHandler::~InputsHandler()
{
	closeJoystick();
}

bool InputsHandler::loadConfig(string const& path)
{
	return true;
}

void InputsHandler::update()
{
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
				case GP_BUTTON_A:
					cout << "bouton 0" << endl;
					break;

					
			}
		}
	}
	//Inputs clavier
	const Uint8* inputClavier = SDL_GetKeyboardState(NULL);
	if (inputClavier[SDL_SCANCODE_UP])
	{
		cout << "up" << endl;
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
	SDL_JoystickClose(m_manette);
	m_manette = NULL;
}