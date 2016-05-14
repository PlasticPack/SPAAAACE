/*	Créateur: Marc-Antoine Céré
Projet: SPAAAACE
Fichier: AiSystem.cpp
But: Créer les méthodes nécessaires à l'utilisation d'une intelligence artificielle
*/

#include "AiSystem.h"

/*
	Constructeur par défaut du système.
	Puisqu'il utilise les attributs des AiComponent, il n'y a pas d'attributs à initialiser.
*/
AiSystem::AiSystem(){}

/*
	Destructeur par défaut du système.
	Puisque le système n'a pas de pointeurs, il n'y a rien à détruire.
*/
AiSystem::~AiSystem(){}

/*
	La méthode update prend le AiComponent donné et le transfer aux méthodes qui
	vont modifiées son mouvement.
	L'utilisation du GameLogicComponent pour donner la puissance des moteurs à l'ai
	est que l'utilisation de pointeurs de manière identique au PhysiqueComponent ne<
	fonctionne pas. Il est donc plus facile de le faire ainsi.
*/
void AiSystem::update(std::shared_ptr <AiComponent> ac, std::shared_ptr <GameLogicComponent> glc){

	ac->setPower(glc->getEnginePower());

	selectionIntel(ac);
}

/*
	Selon le type d'ai donné, soit avec évitement (type 0) ou sans évitement (type 1),
	selectionIntel enverra le component au bon mouvement.
	Si le type donné est inexistant, l'ai sera automatiquement dotée du mouvement avec
	évitement.
*/
void AiSystem::selectionIntel(std::shared_ptr <AiComponent> ac)
{
	if (ac->getType() == 0){
		playIntel(ac);
	}
	else if (ac->getType() == 1){
		playDumb(ac);
	}
	else{
		ac->setType(0);
		selectionIntel(ac);
	}
}


/*
	Le système vérifie si l'intelligence est assez près du joueur pour le poursuivre.
	Sinon, aucun mouvement ne sera fait et l'A.I. restera sur place, quoique toujours
	soumise à la gravité des autres objets.
*/
void AiSystem::verifVision(std::shared_ptr <AiComponent> ac){
	if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) < 10000)
	{
		mouvement(ac);
	}
	else if (ac->getPhysicsComponent()->getPosition().getDist(ac->getTarget()->getPosition()) < 20000){

	}
}

/*
	Le système vérifie si l'intelligence est assez près de l'objet autre que le joueur le
	plus près.
	Sinon, aucun mouvement ne sera fait et l'A.I. restera sur place, quoique toujours
	soumise à la gravité des autres objets.
*/
void AiSystem::verifObject(std::shared_ptr <AiComponent> ac){
	
	if ((ac->getPhysicsComponent()->getPosition().getDist(ac->getNearDanger()->getPosition()) <= (ac->getNearDanger()->getHitboxRadius() + 250)))
	{
		evitement(ac);
	}
}

/*
	Cette fonction a pour but d'éviter les planètes entre l'A.I. et le joueur. L'A.I. prendra donc la normale du vecteur du rayon 
	de la planète vers elle-même plus ce dit vecteur et appliquera sa force de moteur sur ce nouveau vecteur directeur.
	Ainsi, l'A.I. ne peut pas empêcher l'écrasement si elle fonce en ligne droite sur la planète, mais elle peut l'évitée si
	l'angle d'approche est assez grand.
*/
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

	Vec2 vecInverse;
	vecInverse = ac->getPhysicsComponent()->getPosition() - ac->getNearDanger()->getPosition();

	vecDirection = vecDirection + vecInverse.getNormalized();
	
	std::cout << vecDirection.x() << " , " << vecDirection.y() <<" AI" << std::endl;

	ac->getPhysicsComponent()->setForces(vecDirection * ac->getPower());
	
}

/*
	Ce qui fait bouger le joueur. Selon l'angle entre le joueur et elle-même, l'A.I. donnera une "poussée"
	seulement au paramètre (x ou y) qui est le plus grand. Cette technique a pour but d'agrandir la superficie
	que l'A.I. parcours pour attaquer le joueur, augmentant ainsi les chances de collisions.
*/
void AiSystem::mouvement(std::shared_ptr <AiComponent> ac){
	Vec2 vecDirection;

	vecDirection = ac->getTarget()->getPosition() - ac->getPhysicsComponent()->getPosition();
	vecDirection.normalize();;

	if (fabs(vecDirection.x()) > fabs(vecDirection.y())){
		ac->getPhysicsComponent()->setForces(Vec2(vecDirection.x() * ac->getPower(), 0));
	}
	else if (fabs(vecDirection.x()) < fabs(vecDirection.y())){
		ac->getPhysicsComponent()->setForces(Vec2(0, vecDirection.y() * ac->getPower()));
	}

}

/*
	La conscience de l'A.I. Elle commence par vérifié si un objet autre que le joueur se trouve à proximité.
	Si oui, elle oubliera le joueur pour un instant et appliquera son évitement. Sinon, elle foncera
	directement vers le joueur.
*/
void AiSystem::playIntel(std::shared_ptr <AiComponent> ac){
	if (ac->isCloseToDanger()){
		verifObject(ac);
	}
	else{
		verifVision(ac);
	}
}

/*
	Lors du choix de l'intélligence selon le type, si ce type est 1 (donc pas par défaut), L'A.I. se véra attribué 
	une conscience sans l'évitement. L'A.I. se crashera donc sur n'importe quel objet à sa proximité.
*/
void AiSystem::playDumb(std::shared_ptr <AiComponent> ac){
	verifVision(ac);
}
