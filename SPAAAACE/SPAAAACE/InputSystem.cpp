#include "InputSystem.h"
#include <iostream>
#include "InputsComponent.h"
using namespace std;

InputsSystem::InputsSystem() : m_manette(NULL), m_haptic(NULL), m_inputClavier(NULL), m_deadzone(4000)
{
	openJoystick();
}

InputsSystem::~InputsSystem()
{
	m_inputClavier = NULL;
	//destroy sdl_haptic*
	closeJoystick();
}
//à venir
bool InputsSystem::loadConfig(string const& path)
{
	return true;
}
/*
void InputsSystem::update(InputsComponent* inComp)
{
	inComp->update();
}*/

void InputsSystem::pollInputs() 
{
	clearTriggeredAction();
	multimap < int, std::pair<int, double> >::iterator m_actionIt;
	while (SDL_PollEvent(&m_event))
	{
		if (m_event.type == SDL_QUIT)
		{
			m_action.insert(make_pair(AC_EXIT, make_pair(-1, 1)));
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
					if (m_event.jaxis.axis == m_actionIt->second.first - debutGamePadAxisList)
					{
							
								if (abs(m_event.jaxis.value) > m_deadzone || m_event.jaxis.axis > 3)
									m_actionIt->second.second = m_event.jaxis.value;
								else
									m_actionIt->second.second = 0;
					}
					

				}


			}
#pragma endregion Inputs de axes de la manette
#pragma region Inputs
			if (m_event.type == SDL_JOYBUTTONDOWN)
			{
			
				for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
				{
					if (m_event.jbutton.button == m_actionIt->second.first - debutGamePadButtonList) // MOD
					{
						m_actionIt->second.second = 1;// MOD
					}
				}
			}
			else if (m_event.type == SDL_JOYBUTTONUP)
			{
				for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
				{
					if (m_event.jbutton.button == m_actionIt->second.first - debutGamePadButtonList)// MOD
					{
						m_actionIt->second.second = 0;
					}
				}
			}
#pragma endregion Inputs des boutons de manette
		}
	}
	//Inputs clavier
	const Uint8* inputClavier = SDL_GetKeyboardState(NULL);


	for (m_actionIt = m_action.begin(); m_actionIt != m_action.end(); ++m_actionIt)
	{
		if (inputClavier[m_actionIt->second.first])
		{
			m_actionIt->second.second++;
		}
	}

}
//ACTION
double InputsSystem::checkTriggeredAction(int const &actionFlags) 
{
	double value = 0;
	pair<multimap < int, std::pair<int, double> >::iterator, multimap < int, std::pair<int, double> >::iterator> range = m_action.equal_range(actionFlags);
	multimap < int, std::pair<int, double> >::iterator it;
	for (it = range.first; it != range.second; ++it)
	 {
		 if (abs(it->second.second) > value)
			 value = it->second.second;
	 }
	return value;
}

void InputsSystem::setActionTrigger(int const& actionFlag, int const& inputFlag) 
{
	m_action.insert(make_pair(actionFlag, make_pair(inputFlag,0))); 
}

void InputsSystem::clearTriggeredAction() 
{
	multimap<int,std::pair<int,double>>::iterator it = m_action.begin();
	while (it != m_action.end())
	{
		if (it->second.first < debutGamePadButtonList)
			it->second.second = 0; 
			++it;
	}
}

void InputsSystem::eraseInputsInAction(int const& actionFlag)
{
	m_action.erase(actionFlag);
}

//JOYSTICk FUNCTION
bool InputsSystem::isJoyConnected()
{
	return m_manette != NULL;
}

void InputsSystem::rumbleJoy(unsigned temps, double puissance)
{
	if (SDL_HapticRumblePlay(m_haptic, puissance, temps) != 0)
	{
		cout << "unable to rumble!" << endl;
	}
}

bool InputsSystem::openJoystick()
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

void InputsSystem::closeJoystick()
{
	SDL_HapticClose(m_haptic);
	m_haptic = NULL;
	SDL_JoystickClose(m_manette);
	m_manette = NULL;
}

void InputsSystem::setJoyDeadzone(unsigned const& deadzone)
{
	m_deadzone = deadzone;
}
unsigned InputsSystem::getJoyDeadzone()
{
	return m_deadzone;
}