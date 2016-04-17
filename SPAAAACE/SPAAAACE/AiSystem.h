#pragma once

#include "AiComponent.h"

class AiSystem{
public:

	AiSystem();
	~AiSystem();
	static void update(std::shared_ptr <AiComponent> ac, std::vector <std::shared_ptr <PhysicsComponent>> &physics, std::shared_ptr <PhysicsComponent> target);
	static void verifVision(std::shared_ptr <AiComponent> ac);
	static void verifObject(std::shared_ptr <AiComponent> ac);
	static void evitement(std::shared_ptr <AiComponent> ac);
	static void mouvementCont(std::shared_ptr <AiComponent> ac);
	static void mouvementAcc(std::shared_ptr <AiComponent> ac);
	static double decc(double vit, double acc, std::shared_ptr <AiComponent> ac);
	static void selectionMouvement(std::shared_ptr <AiComponent> ac);
	static void play(std::shared_ptr <AiComponent> ac);

};