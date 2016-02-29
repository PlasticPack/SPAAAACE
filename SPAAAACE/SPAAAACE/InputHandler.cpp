#include "InputHandler.h"
#include <iostream>

using namespace std;

InputsHandler::InputsHandler() : m_manette(NULL), m_haptic(NULL), m_inputClavier(NULL), m_deadzone(4000)
{
	for (int i = debutActionList; i < AC_NUMBER_OF_ACTION; i++)
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
	clearTriggeredAction();
	map<int, int>::iterator m_actionIt;
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
#pragma region Inputs
			if (m_event.type == SDL_JOYAXISMOTION)
			{
				if (m_event.jaxis.which == 0)
				{
					for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
					if (m_event.jaxis.axis == m_actionIt->second - debutGamePadAxisList)
					{
						if (abs(m_event.jaxis.value) > m_deadzone || m_event.jaxis.axis > 3 )
						m_trigeredAction[m_actionIt->first] = m_event.jaxis.value;
					}
					

				}


			}
#pragma endregion Inputs de axes de la manette
#pragma region Inputs
			if (m_event.type == SDL_JOYBUTTONDOWN)
			{
			
				for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
				{
					if (m_event.jbutton.button == m_actionIt->second - debutGamePadButtonList)
					{
						m_trigeredAction[m_actionIt->first] = 1;
					}
				}
			}
			else if (m_event.type == SDL_JOYBUTTONUP)
			{
				for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
				{
					if (m_event.jbutton.button == m_actionIt->second - debutGamePadButtonList)
					{
						m_trigeredAction.erase(m_actionIt->first);
					}
				}
			}
#pragma endregion Inputs des bouton de manette
		}
	}
	//Inputs clavier
	const Uint8* inputClavier = SDL_GetKeyboardState(NULL);


	for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
	{
		if (inputClavier[m_actionIt->second])
		{
			m_trigeredAction[m_actionIt->first] = 1;
		}
	}
}
//ACTION
double InputsHandler::checkTriggeredAction(int const &flags)
{
	return m_trigeredAction[flags];
}

void InputsHandler::setActionTrigger(int const& actionFlag, int const& inputFlag)
{
	m_action[actionFlag] = inputFlag;
}

void InputsHandler::clearTriggeredAction()
{
	map<int, double>::iterator it = m_trigeredAction.begin();
	while (it != m_trigeredAction.end())
	{
		if (m_action[it->first] < debutGamePadButtonList)
			m_trigeredAction.erase(it++);
		else
			++it;
	}
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