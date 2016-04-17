#include "AiSystem.h"

AiSystem::AiSystem(){}

AiSystem::~AiSystem(){}

void AiSystem::update(std::shared_ptr <AiComponent> ac, std::vector<std::shared_ptr<PhysicsComponent>>	&physics , std::shared_ptr <PhysicsComponent> target){
	ac->setTarget(target);
	ac->setNearDanger(physics[1]);
	
	for (int j = 1; j < physics.size(); j++) {
		if (physics[j] != target){
			if (ac->getPhysicsComponent()->getPosition().getDist(ac->getNearDanger()->getPosition()) >= ac->getPhysicsComponent()->getPosition().getDist(physics[j]->getPosition())){
				ac->setNearDanger(physics[j]);
			}
		}
	}

	play(ac);
}


void AiSystem::selectionMouvement(std::shared_ptr <AiComponent> ac)
{
	if (ac->getType() == 1){
		mouvementAcc(ac);
	}
	else if (ac->getType() == 0){
		mouvementCont(ac);
	}
	else{
		ac->setType(0);
		selectionMouvement(ac);
	}
}

void AiSystem::verifVision(std::shared_ptr <AiComponent> ac){
	/*if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) <= 1000){
		mouvementAcc(ac);
	}
	else if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) == 1000){
		Vec2 vecTest;
		vecTest = ac->getTarget()->getPosition() - ac->getPhysicsComponent()->getPosition();
		vecTest.normalize();
		ac->getPhysicsComponent()->setVelocity(vecTest*1500);
	}
	else*/ if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) < 1500)
	{
		mouvementCont(ac);
	}
	else{
		ac->getPhysicsComponent()->setPosition(ac->getTarget()->getPosition() + Vec2(250,250));
		ac->getPhysicsComponent()->setVelocity(Vec2(0,0));
	}

	
}

void AiSystem::verifObject(std::shared_ptr <AiComponent> ac){
	if (ac->getPhysicsComponent()->getPosition().getDist(ac->getNearDanger()->getPosition()) <= (ac->getNearDanger()->getHitboxRadius() + 100))
	{
		evitement(ac);
	}
	std::cout << ac->getNearDanger()->getHitboxRadius() + 200 << std::endl;
}

void AiSystem::evitement(std::shared_ptr <AiComponent> ac){
	std::cout << "DANGER" << std::endl;

	Vec2 vecDirection;

	vecDirection = ac->getNearDanger()->getPosition() - ac->getPhysicsComponent()->getPosition();
	vecDirection.normalize();

	ac->getPhysicsComponent()->setForces(vecDirection * -10000);
}

void AiSystem::mouvementCont(std::shared_ptr <AiComponent> ac){
	Vec2 vecDirection;

	vecDirection = ac->getTarget()->getPosition() - ac->getPhysicsComponent()->getPosition();
	vecDirection.normalize();;

	if (fabs(vecDirection.x()) > fabs(vecDirection.y())){
		ac->getPhysicsComponent()->setForces(Vec2(vecDirection.x() * 500, 0));

	}
	else if (fabs(vecDirection.x()) < fabs(vecDirection.y())){
		ac->getPhysicsComponent()->setForces(Vec2(0, vecDirection.y() * 500));
	}
	/*else{
		ac->getPhysicsComponent()->setForces(vecTest.operator*(1500));
	}*/

	/*if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) == 500)
	{
		ac->getPhysicsComponent()->setForces(Vec2(ac->getPhysicsComponent()->getVelocity()*-1));
	}*/

	
	/*if (ac->getVec().getDist(ac->getTarget()->getPosition()) <= 1000)// pour déccélération
	{
	vecTest.operator=(ac->getTarget()->getPosition().operator-(ac->getPhysicsComponent()->getPosition()));
	vecTest.normalize();
	ac->getPhysicsComponent()->setForces(vecTest.operator*(-1900));


	}
	else{*/
	//	ac->getPhysicsComponent()->setForces(ac->getPhysicsComponent()->getForces()+vecTest.operator*(750));
	//}
	


}


void AiSystem::mouvementAcc(std::shared_ptr <AiComponent> ac){
	Vec2 vecDirection;


	vecDirection = ac->getTarget()->getPosition() - ac->getPhysicsComponent()->getPosition();
	vecDirection.normalize();
	ac->getPhysicsComponent()->setForces(vecDirection * 500);
	/*if (ac->getVec().getDist(ac->getTarget()->getPosition()) <= decc(m_vit, m_acc) / 2)// pour déccélération
	{
	vecTest.operator=(ac->getTarget()->getPosition().operator-(ac->getPhysicsComponent()->getPosition()));
	vecTest.normalize();
	ac->getPhysicsComponent()->setForces(vecTest.operator*(-1500));


	}
	else if (m_vit<60)// Exemple de plafond de vitesse et accélération si sous le plafond
	{
	vecTest.operator=(ac->getTarget()->getPosition().operator-(ac->getPhysicsComponent()->getPosition()));
	vecTest.normalize();
	m_vit += m_acc;
	ac->getPhysicsComponent()->setForces(vecTest.operator*(1500));
	}
	else // Vitesse continue
	{
	vecTest.operator=(ac->getTarget()->getPosition().operator-(ac->getPhysicsComponent()->getPosition()));
	vecTest.normalize();
	ac->getPhysicsComponent()->setForces(vecTest.operator*(1500));
	}*/

}

/*
Revoie la distance minimale pour déccélérer selon
l'acc (qui est l'équivalent de décc) et la vitesse
*/
/*double AiSystem::decc(double vit, double acc, std::shared_ptr <AiComponent> ac)
{
	int n = ceil(vit / (double)acc);
	double tot = 0;
	for (int i = 1; i <= n; i++)
	{
		tot += i*acc;
	}
	return tot;
}*/

void AiSystem::play(std::shared_ptr <AiComponent> ac){
	verifVision(ac);
	verifObject(ac);
}