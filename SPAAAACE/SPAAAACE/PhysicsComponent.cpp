#include "PhysicsComponent.h"

//Les blagues ont une grasieuster de Justin :)


PhysicsComponent::PhysicsComponent(std::shared_ptr<PositionComponent> c)
{
	m_activated = true;
	m_posComponent = c;
	m_mass = 1.0;
	//Si j'ai trois jeunes dans une autobus qui va vers l'école combien il a de jambre dans cette autobus ?
	m_hitboxRadius = 50;
}


PhysicsComponent::~PhysicsComponent()
{
	//Pète pî répète s'en vont en bateau. Pète tombe a l'eau, qui qui reste. 
	//WHILE LOOP !!!!!!!
	//while(true);
}


Vec2 PhysicsComponent::getVelocity(){
	return m_velocity;
}

Vec2 PhysicsComponent::getPosition(){
	return m_posComponent->getPosition();
}

Vec2 PhysicsComponent::getForces(){
	return m_forces;
}

double PhysicsComponent::getMass(){
	return m_mass;
	//Gab fait de la musique si sort des auparleurs
}

double PhysicsComponent::getHitboxRadius(){
	return m_hitboxRadius;
}

std::shared_ptr<PositionComponent> PhysicsComponent::getPositionComponent(){
	return m_posComponent;
}

void PhysicsComponent::setPosition(Vec2 pos){
	m_posComponent->setPosition(pos);
}

void PhysicsComponent::setVelocity(Vec2 pos){
	m_velocity = pos;
}

void PhysicsComponent::setForces(Vec2 pos){
	m_forces = pos;
	//LUA LUA LUA LUA LUA LUALUALUA  LUA LUA LUALUA LUA LUALUA  v  vLUA LUALUALUA LUA LUA LUALUA LUA LUA LUALUA v LUA
}

void PhysicsComponent::setMass(double pos){
	m_mass = pos;
}

void PhysicsComponent::setHitboxRadius(double pos){
	m_hitboxRadius = pos;
}
/*
void PhysicsComponent::update(std::vector<PhysicsComponent> &phyComps, double dt){
	Derivative a, b, c, d;
	//std::cout << "Updatin' them phyzix! Pos of da lil' guy : " << m_posComponent->getPosition().x() << "\n";
	//on évalue 4 fois pour avoir une idée de la courbe
	//de l'accélération (si elle n'est pas constante, 
	//comme dans le cas d'une attraction gravitationnelle)


	/*for (int i(0); i < phyComps.size(); i++) {
		for (int j(i); j < phyComps.size(); j++) {
			if (i != j)
				collide(phyComps[i], phyComps[j], dt);
		}
	}

	a = evaluate(phyComps, 0.0, Derivative());
	b = evaluate(phyComps, 0.5 * dt, a);
	c = evaluate(phyComps, 0.5 * dt, b);
	d = evaluate(phyComps, dt, c);

	Vec2 dposdt = (a.dpos + ((b.dpos + c.dpos) * 2) + d.dpos) * (1.0 / 6.0);
	Vec2 dveldt = (a.dvel + ((b.dvel + c.dvel) * 2) + d.dvel) * (1.0 / 6.0);

	setPosition(getPosition() + (dposdt * dt));
	setVelocity(getVelocity() + (dveldt * dt));

	setForces(Vec2(0, 0));
}

Vec2 PhysicsComponent::gravity(PhysicsComponent &b) {
	//F = GMM/r^2
	if (m_posComponent->getZIndex() != 0 || b.m_posComponent->getZIndex() != 0){
		double G = 5000;

		double r = (getPosition().getDist(b.getPosition())); //on obtient la distance entre les 2 points
		double deltaZ = m_posComponent->getZIndex() - b.m_posComponent->getZIndex();
		deltaZ *= 100; // pour amplifier la différence de niveau
		r = sqrt((r * r) + (deltaZ * deltaZ)); // si la différence de hauteur des objets est trop importante, l'attraction est réduite

		Vec2 vec = (getPosition() - b.getPosition()); // le vecteur entre les 2
		vec.normalize(); // unitaire pcq son module est l'attraction

		double attraction = (-G * b.getMass() / (r * r)); //le module de l'attraction gravitationnelle
		return vec * attraction;
	}
	else{
		return Vec2(0, 0);
	}
}


Vec2 PhysicsComponent::accelerate(std::vector<PhysicsComponent> &phyComps, double dt){
	//Ques qui est plus vite ?
	//a ) La lumière 
	//b ) La pensé 
	//c) L,envie de chier

	//Voir la réponce plus bas

	Vec2 totalGravity;
	for (int i = 0; i < phyComps.size(); i++){
		if (this->m_posComponent != phyComps[i].m_posComponent){ // si on pointe pas vers le même component de position
			if (/*getPosition().getDist(phyComps[i].getPosition())  <  2000 && !checkIfCollide(phyComps[i], dt)) { // si l'objet est assez proche ET qu'ils sont pas l'un dans l'autre
				totalGravity += gravity(phyComps[i]);
			}
		}
	}

	m_forces += totalGravity;

	//std::cout << "\nLKHSABD : " << m_forces.getLength() << " / " << m_mass << " = " << (m_forces / m_mass).getLength() <<   "\n";

	return m_forces / m_mass;
}

Derivative PhysicsComponent::evaluate(std::vector<PhysicsComponent> &phyComps, double dt, Derivative &d){

	State bState;
	bState.pos = (getPosition() + (d.dpos * dt)); // on avance la position de l'objet de dt secondes dans le temps (en utilisant la vélocité de la dérivée) (vitesse * temps = déplacement)
	bState.vel =(getVelocity() + (d.dvel * dt)); // même principe ici, mais pour l'accélération
	//C'était une fois une gars qui voulais entre dans la police, la policer c'est tasser et le gars est enter dans le mur
	Derivative output;
	output.dpos = getVelocity() + (d.dvel * dt);
	output.dvel = accelerate(phyComps, dt);
	return output;


	//RÉPONCE : L'envie de chier est le plus vite puisque que quand tu a l'envier de chier tu n'es pas le temps de penser d'ouvir la lumière.
}

void collide(PhysicsComponent &a, PhysicsComponent &b, double dt){
	if (a.checkIfCollide(b, dt) && b.checkIfCollide(a, dt) /*et z index) { //s'ils se touchent

		std::cout << "HEY";

		//on sort les 2 objets l'un de l'autre
		Vec2 direction = (a.getPosition() - b.getPosition()).getNormalized();

		direction *= a.getHitboxRadius() + b.getHitboxRadius();
		Vec2 deplacement = ((direction - (a.getPosition() - b.getPosition())) / 2);

		a.setPosition(a.getPosition() + deplacement);
		b.setPosition(b.getPosition() + deplacement*-1);


		//on calcule la vélocité des 2 après la collision
		double firstA = (2 * b.getMass() / (a.getMass() + b.getMass()));
		double secondA = (a.getVelocity() - b.getVelocity()) * (a.getPosition() - b.getPosition()) / ((a.getPosition() - b.getPosition()).getLength() * (a.getPosition() - b.getPosition()).getLength());

		double firstB = (2 * a.getMass() / (a.getMass() + b.getMass()));
		double secondB = (b.getVelocity() - a.getVelocity()) * (b.getPosition() - a.getPosition()) / ((b.getPosition() - a.getPosition()).getLength() * (b.getPosition() - a.getPosition()).getLength());

		Vec2 finalVelA = a.getVelocity() - ((a.getPosition() - b.getPosition())* (firstA * secondA));
		Vec2 finalVelB = b.getVelocity() - ((b.getPosition() - a.getPosition())* (firstB * secondB));

		//double elasticity = a.getElasticity() * b.getElasticity();

		//std::cout << elasticity << "\n";

		//a.setVelocity(finalVelA );
		//b.setVelocity(finalVelB );

		//std::cout << a.getVelocity().getLength() << "// B :" << b.getVelocity().getLength() << "\n";
	}
}*/