#include "AiSystem.h"

AiSystem::AiSystem(){}

AiSystem::~AiSystem(){}

void AiSystem::update(std::shared_ptr <AiComponent> ac){

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
	else*/
	if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) < 100000)
	{
		mouvementCont(ac);
	}
	/*else{
		ac->getPhysicsComponent()->setPosition(ac->getTarget()->getPosition() + Vec2(250,250));
		ac->getPhysicsComponent()->setVelocity(Vec2(0,0));
	}*/

	
}

void AiSystem::verifObject(std::shared_ptr <AiComponent> ac){
/*	if ((ac->getPhysicsComponent()->getPosition().getDist(ac->getNearDanger()->getPosition()) <= (ac->getNearDanger()->getHitboxRadius() + 1000)) && (ac->getPhysicsComponent()->getPosition().getDist(ac->getNearDanger()->getPosition()) >= (ac->getNearDanger()->getHitboxRadius() + 500))){
		decc(ac);
	}
	else*/ if ((ac->getPhysicsComponent()->getPosition().getDist(ac->getNearDanger()->getPosition()) <= (ac->getNearDanger()->getHitboxRadius() + 500)))
	{
		evitement(ac);
	}
}

void AiSystem::evitement(std::shared_ptr <AiComponent> ac){

	Vec2 vecDirection;

	vecDirection = ac->getPhysicsComponent()->getPosition() - ac->getNearDanger()->getPosition();
	vecDirection.normalize();
	int x, y;

	x = vecDirection.x();
	y = vecDirection.y();

	
	if (x <= y){
		if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition() + Vec2(-y, x)) < ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition() + Vec2(y, x))){
			vecDirection.setCoords(-y, x);
		}
		else{
			vecDirection.setCoords(y, x);
		}
	}
	else if (x > y){
		if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition() + Vec2(y, -x)) < ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition() + Vec2(y, x))){
			vecDirection.setCoords(y, -x);
		}
		else{
			vecDirection.setCoords(y, x);
		}
	}

	vecDirection = vecDirection + Vec2(ac->getNearDanger()->getPosition() - ac->getPhysicsComponent()->getPosition()).getNormalized();
	
	std::cout << vecDirection.x() << " , " << vecDirection.y() <<" AI" << std::endl;

	ac->getPhysicsComponent()->setForces(vecDirection *2000);
	
	

}

void AiSystem::mouvementCont(std::shared_ptr <AiComponent> ac){
	Vec2 vecDirection;

	vecDirection = ac->getTarget()->getPosition() - ac->getPhysicsComponent()->getPosition();
	vecDirection.normalize();;

	if (fabs(vecDirection.x()) > fabs(vecDirection.y())){
		ac->getPhysicsComponent()->setForces(Vec2(vecDirection.x() * 2000, 0));
		

	}
	else if (fabs(vecDirection.x()) < fabs(vecDirection.y())){
		ac->getPhysicsComponent()->setForces(Vec2(0, vecDirection.y() * 2000));
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


void AiSystem::decc(std::shared_ptr <AiComponent> ac)
{
	Vec2 vecDirection;

	vecDirection = ac->getPhysicsComponent()->getPosition() - ac->getNearDanger()->getPosition();
	vecDirection.normalize();
	//if (ac->getPhysicsComponent()->getVelocity().getLength() > 200){
		ac->getPhysicsComponent()->setForces(vecDirection * -2000);
	//}
	

}

void AiSystem::play(std::shared_ptr <AiComponent> ac){
	if (ac->isCloseToDanger()){
		verifObject(ac);
	}
	else{
		verifVision(ac);
	}
}

