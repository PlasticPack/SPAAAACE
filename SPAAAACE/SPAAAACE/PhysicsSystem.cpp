#include "PhysicsSystem.h"

double PhysicsSystem::m_gravity = 6.67 * pow(10, 4);

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::resolveCollision(Message &postman, Scene *s, std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt) {
	if (checkIfCollide(postman, s, a, b, dt) && a->getPositionComponent()->getZIndex() == b->getPositionComponent()->getZIndex() && a->isActive() && b->isActive()) { //s'ils se touchent
		
		//on sort les 2 objets l'un de l'autre
		Vec2 direction = (a->getPosition() - b->getPosition()).getNormalized();

		direction *= a->getHitboxRadius() + b->getHitboxRadius();
		Vec2 deplacement = ((direction - (a->getPosition() - b->getPosition())) / 2);

		a->setPosition(a->getPosition() + deplacement);
		b->setPosition(b->getPosition() + deplacement*-1);

		postman.addMessage(s->getFatherID<PhysicsComponent>(a), s->getFatherID<PhysicsComponent>((int)b.get()), MS_COLLISION, (a->getVelocity().getLength() + b->getVelocity().getLength()));
		postman.addMessage(s->getFatherID<PhysicsComponent>(b), s->getFatherID<PhysicsComponent>((int)a.get()), MS_COLLISION, (a->getVelocity().getLength() + b->getVelocity().getLength())); // on envoie l'adresse de l'autre

		double elasticity = a->getElasticity() * b->getElasticity();

		//on calcule la v�locit� des 2 apr�s la collision
		double firstA = (2 * b->getMass() / (a->getMass() + b->getMass()));
		double secondA = (((a->getVelocity() - b->getVelocity()) * elasticity) * (a->getPosition() - b->getPosition())) / ((a->getPosition() - b->getPosition()).getLength() * (a->getPosition() - b->getPosition()).getLength());

		double firstB = (2 * a->getMass() / (a->getMass() + b->getMass()));
		double secondB = (((b->getVelocity() - a->getVelocity()) * elasticity) * (b->getPosition() - a->getPosition())) / ((b->getPosition() - a->getPosition()).getLength() * (b->getPosition() - a->getPosition()).getLength());

		Vec2 finalVelA = a->getVelocity() - ((a->getPosition() - b->getPosition())* (firstA * secondA));
		Vec2 finalVelB = b->getVelocity() - ((b->getPosition() - a->getPosition())* (firstB * secondB));

		// angular momentum

		if (finalVelA.getAngle(finalVelB) != 0) {
			//a->setAngularVelocity((finalVelA.getAngle(finalVelB) / abs(finalVelA.getAngle(finalVelB))) * finalVelA.getLength() / a->getHitboxRadius());
			//b->setAngularVelocity((finalVelB.getAngle(finalVelA) / abs(finalVelB.getAngle(finalVelA))) * finalVelB.getLength() / b->getHitboxRadius());
		}
		
		
		a->setVelocity(finalVelA);
		b->setVelocity(finalVelB);

	}
}

bool PhysicsSystem::checkIfCollide(Message &postman, Scene *s, std::shared_ptr<PhysicsComponent> a, std::shared_ptr<PhysicsComponent> b, double dt){ // check seulement s'ils OVERLAP , fuck leur ZINDEX 
	if (&a != &b){
		//si les objets vont tr�s vite, il se peut qu'un objet "passe � travers" l'autre"
		//pour �viter ca, on regarde la position de l'objet par petits bond jusqu'� la
		//prochaine frame

		//option #2, plus optimis�e : faire 2 segments avec le d�placement des objets et checker s'ils intersectent
		//plus optimis� pcq pas besoin de faire une boucle 
		//mais plus long � trouver la bonne impl�mentation (et plus compliqu�)
		//donc je me suis dit fuck it, on y va pour ca pour tout de suite

		bool detected = false;
		double accuracy = 2; // la pr�cision ou le nombre de bonds qu'on fait 
		for (int i = 1; i <= accuracy; i++){
			if (a->getHitboxRadius() + b->getHitboxRadius() > (a->getPosition() + (a->getVelocity() * (i / accuracy) *dt)).getDist((b->getPosition()) + (b->getVelocity() * (i / accuracy) *dt))){
				detected = true;
				i = accuracy + 1;
			}
		}
		if (detected ){


			//collision entre joueur et obj.
			if (s->getFatherID<PhysicsComponent>(a) == "player"){
				if (s->getFatherID<PhysicsComponent>(b).find("objective")){
					//postman.addMessage();
				}
			}

			//std::cout << "DETECTED COLLISION\n";
			if (!a->isActive() || !b->isActive()) {
				//postman.addMessage(s->getFatherID<PhysicsComponent>(a), s->getFatherID<PhysicsComponent>((int)b.get()), MS_COLLISION, 1);
				//postman.addMessage(s->getFatherID<PhysicsComponent>(b), s->getFatherID<PhysicsComponent>((int)a.get()), MS_COLLISION, 1);
			}
		}
		else {

		}
		return detected;
	}
	else
		return false;
}

bool PhysicsSystem::checkIfCollide(PhysicsComponent &a, PhysicsComponent &b, double dt){ // check seulement s'ils OVERLAP , fuck leur ZINDEX 
	if (&a != &b){
		//si les objets vont tr�s vite, il se peut qu'un objet "passe � travers" l'autre"
		//pour �viter ca, on regarde la position de l'objet par petits bond jusqu'� la
		//prochaine frame

		//option #2, plus optimis�e : faire 2 segments avec le d�placement des objets et checker s'ils intersectent
		//plus optimis� pcq pas besoin de faire une boucle 
		//mais plus long � trouver la bonne impl�mentation (et plus compliqu�)
		//donc je me suis dit fuck it, on y va pour ca pour tout de suite

		bool detected = false;
		double accuracy = 2; // la pr�cision ou le nombre de bonds qu'on fait 
		for (int i = 1; i <= accuracy; i++){
			if (a.getHitboxRadius() + b.getHitboxRadius() > (a.getPosition() + (a.getVelocity() * (i / accuracy) *dt)).getDist((b.getPosition()) + (b.getVelocity() * (i / accuracy) *dt))){
				detected = true;
				i = accuracy + 1;
			}
		}
		return detected;
	}
	else
		return false;
}

Vec2 PhysicsSystem::gravity(PhysicsComponent &a, PhysicsComponent &b) {
	if (a.isActive() && b.isActive()){
		double G = m_gravity;

		double r = (a.getPosition().getDist(b.getPosition())); //on obtient la distance entre les 2 points

		double deltaZ = a.getPositionComponent()->getZIndex() - b.getPositionComponent()->getZIndex();
		deltaZ *= deltaZ * 1000; // pour amplifier la diff�rence de niveau
		r = sqrt((r * r) + (deltaZ * deltaZ)); // si la diff�rence de hauteur des objets est trop importante, l'attraction est r�duite

		Vec2 vec = (a.getPosition() - b.getPosition()); // le vecteur entre les 2
		vec.normalize(); // unitaire pcq son module est l'attraction

		double attraction = (-G * a.getMass() * b.getMass() / (r * r)); //le module de l'attraction gravitationnelle

		return vec * attraction;
	}
	return Vec2(0,0);
}

Derivative PhysicsSystem::evaluate(PhysicsComponent &initial, std::vector<std::shared_ptr<PhysicsComponent>> &bodies, double dt, Derivative &d){
	

	std::shared_ptr<PositionComponent> bPos = std::make_shared<PositionComponent>(*initial.getPositionComponent());

	PhysicsComponent bState(initial.getPositionComponent());
	bState.setPosition(initial.getPosition() + (d.dpos * dt)); // on avance la position de l'objet de dt secondes dans le temps (en utilisant la v�locit� de la d�riv�e) (vitesse * temps = d�placement)
	bState.setVelocity(initial.getVelocity() + (d.dvel * dt)); // m�me principe ici, mais pour l'acc�l�ration
	bState.setMass(initial.getMass());
	bState.setForces(initial.getForces());
	bState.setHitboxRadius(initial.getHitboxRadius());

	Derivative output;
	output.dpos = initial.getVelocity() + (d.dvel * dt);
	output.dvel = accelerate(initial, bState, bodies, dt);
	return output;
}

void PhysicsSystem::update(Message &postman, Scene *s, PhysicsComponent &body, std::vector<std::shared_ptr<PhysicsComponent>> &bodies, double dt){
	Derivative a, b, c, d;
	//on �value 4 fois pour avoir une id�e de la courbe
	//de l'acc�l�ration (si elle n'est pas constante, 
	//comme dans le cas d'une attraction gravitationnelle)

	//std::cout << "VEL OF " <<  &body << " : " << body.getVelocity().getLength() << "\n";
	if (body.isActive()){
		for (int i(0); i < bodies.size(); i++) {
			for (int j(i); j < bodies.size(); j++) {
				if (i != j){
					resolveCollision(postman, s, bodies[i], bodies[j], dt);
				}
			}
		}

		a = evaluate(body, bodies, 0.0, Derivative());
		b = evaluate(body, bodies, 0.5 * dt, a);
		c = evaluate(body, bodies, 0.5 * dt, b);
		d = evaluate(body, bodies, dt, c);

		Vec2 dposdt = (a.dpos + ((b.dpos + c.dpos) * 2) + d.dpos) * (1.0 / 6.0);
		Vec2 dveldt = (a.dvel + ((b.dvel + c.dvel) * 2) + d.dvel) * (1.0 / 6.0);

		body.setPosition(body.getPosition() + (dposdt * dt));
		body.setVelocity(body.getVelocity() + (dveldt * dt));

		body.setForces(Vec2(0, 0));

		body.getPositionComponent()->setAngle(body.getPositionComponent()->getAngle() + body.getAngularVelocity() * dt);
		//std::cout << body.getAngularVelocity() * dt << "\n";
	}
	else {
		body.setPosition(body.getPosition() + (body.getVelocity() * dt));
	}
}

Vec2 PhysicsSystem::accelerate(PhysicsComponent& initial, PhysicsComponent &b, std::vector<std::shared_ptr<PhysicsComponent>> &bodies, double dt){
	//retourne l'acc�l�ration selon l'�tat (position) et le temps

	if (b.isActive()){
		//calcule aussi la gravit� de tous les autres corps
		//sur ce corps-ci, si ils sont assez proche
		Vec2 totalGravity;
		//if (!initial.isFixed() && initial.isSolid()){ // si l'objet est affect� par la gravit�
		for (int i(0); i < bodies.size(); i++){
			if (bodies[i].get() != &initial){ // si l'objet est diff�rent de lui-m�me
				if (!checkIfCollide(initial, *bodies[i], dt)) { // si l'objet est assez proche ET qu'ils sont pas l'un dans l'autre
					totalGravity += gravity(b, *bodies[i]);
				}
				//std::cout << "GRAV";
			}
		}
		//}

		initial.setForces(initial.getForces() + totalGravity);
		b.setForces(initial.getForces());
		return (b.getForces()) / (b.getMass());
	}
	else
		return Vec2(0, 0);
}