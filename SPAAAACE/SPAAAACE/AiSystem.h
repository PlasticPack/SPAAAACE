/*	Créateur: Marc-Antoine Céré
Projet: SPAAAACE
Fichier: AiSystem.h
But: Prototype de la classe AiSystem
*/
#pragma once
#include "AiComponent.h"

class AiSystem{
public:

	AiSystem();
	~AiSystem();
	static void update(std::shared_ptr <AiComponent> ac,  std::shared_ptr <GameLogicComponent> glc);
	static void verifVision(std::shared_ptr <AiComponent> ac);
	static void verifObject(std::shared_ptr <AiComponent> ac);
	static void evitement(std::shared_ptr <AiComponent> ac);
	static void mouvement(std::shared_ptr <AiComponent> ac);
	static void decc(std::shared_ptr <AiComponent> ac);
	static void selectionIntel(std::shared_ptr <AiComponent> ac);
	static void playIntel(std::shared_ptr <AiComponent> ac);
	static void playDumb(std::shared_ptr <AiComponent> ac);

};
