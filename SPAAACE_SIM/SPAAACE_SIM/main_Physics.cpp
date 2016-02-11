#include "main_Physics.h"

namespace Physics {
	
	void collide(Body &a, Body &b, double dt) { 

		if (checkIfCollide(a, b, dt) && a.isSolid() && b.isSolid() ) { //s'ils se touchent
			
			//on sort les 2 objets l'un de l'autre
			Vec2 direction = (a.getPosition() - b.getPosition()).getNormalized();

			direction *= a.getHitboxRadius() + b.getHitboxRadius();
			Vec2 deplacement = ((direction - (a.getPosition() - b.getPosition())) / 2);

			a.setPosition(a.getPosition() + deplacement);
			b.setPosition(b.getPosition() + deplacement*-1);

			
			//on calcule la v�locit� des 2 apr�s la collision
			double firstA = (2 * b.getMass() / (a.getMass() + b.getMass()));
			double secondA = (a.getVelocity() - b.getVelocity()) * (a.getPosition() - b.getPosition()) / ((a.getPosition() - b.getPosition()).getLength() * (a.getPosition() - b.getPosition()).getLength());

			double firstB = (2 * a.getMass() / (a.getMass() + b.getMass()));
			double secondB = (b.getVelocity() - a.getVelocity()) * (b.getPosition() - a.getPosition()) / ((b.getPosition() - a.getPosition()).getLength() * (b.getPosition() - a.getPosition()).getLength());

			Vec2 finalVelA = a.getVelocity() - ((a.getPosition() - b.getPosition())* (firstA * secondA));
			Vec2 finalVelB = b.getVelocity() - ((b.getPosition() - a.getPosition())* (firstB * secondB));

			double elasticity = a.getElasticity() * b.getElasticity();

			std::cout << elasticity <<"\n";

			a.setVelocity(finalVelA * elasticity);
			b.setVelocity(finalVelB * elasticity);
		}
	}

	bool checkIfCollide(Body &a, Body &b, double dt){
		if (&a != &b){
			//si les objets vont tr�s vite, il se peut qu'un objet "passe � travers" l'autre"
			//pour �viter ca, on regarde la position de l'objet par petits bond jusqu'� au prochain quart de frame

			//option #2, plus optimis�e : faire 2 segments avec le d�placement des objets et checker s'ils intersectent
			//plus optimis� pcq pas besoin de faire une boucle 
			//mais plus long � trouver la bonne impl�mentation (et plus compliqu�)
			//donc je me suis dit fuck it, on y va pour ca pour tout de suite

			bool detected = false;
			/*double accuracy = 10; // la pr�cision ou le nombre de bonds qu'on fait 
			for (int i = 0; i <= accuracy; i++){
				if (a.getHitboxRadius() + b.getHitboxRadius() > ((a.getPosition() ) + (a.getVelocity() * (0.75 * i / accuracy) *dt)).getDist((b.getPosition()) + (b.getVelocity() * (0.75 * i / accuracy) *dt))){
					detected = true;
					i = accuracy + 1;
				}
			}*/

			if (a.getHitboxRadius() + b.getHitboxRadius() > a.getPosition().getDist(b.getPosition())){
				detected = true;
			}
			return detected;
		}
		else
			return false;
	}

	Vec2 gravity(Body &a, Body &b) {
		//F = GMM/r^2
		
		double G = 5000;
		
		double r = (a.getPosition().getDist(b.getPosition())); //on obtient la distance entre les 2 points

		Vec2 vec = (a.getPosition() - b.getPosition()); // le vecteur entre les 2
		vec.normalize(); // unitaire pcq son module est l'attraction

		double attraction = (-G * a.getMass() * b.getMass() / (r * r)); //le module de l'attraction gravitationnelle

		return vec * attraction;
	} 

	Derivative evaluate(Body &initial, std::vector<Body> &bodies, double t, double dt, Derivative &d){

		Body bState;
		bState.setPosition(initial.getPosition() + (d.dpos * dt)); // on avance la position de l'objet de dt secondes dans le temps (en utilisant la v�locit� de la d�riv�e) (vitesse * temps = d�placement)
		bState.setVelocity(initial.getVelocity() + (d.dvel * dt)); // m�me principe ici, mais pour l'acc�l�ration
		bState.setMass(initial.getMass());
		bState.setForces(initial.getForces());
		bState.setHitboxRadius(initial.getHitboxRadius());
		bState.setSolid(initial.isSolid());

		Derivative output;
		output.dpos = initial.getVelocity() + (d.dvel * dt);
		output.dvel = accelerate(initial, bState, bodies, t + dt);
		return output;
	}

	//le t est facultatif pour le moment
	void move(Body &body, std::vector<Body> &bodies, double t, double dt) {
		Derivative a, b, c, d;
		//on �value 4 fois pour avoir une id�e de la courbe
		//de l'acc�l�ration (si elle n'est pas constante, 
		//comme dans le cas d'une attraction gravitationnelle)


		for (int i(0); i < bodies.size(); i++) {
			for (int j(i); j < bodies.size(); j++) {
				if (i != j)
					Physics::collide(bodies[i], bodies[j], dt);
			}
		}
		 
		a = evaluate(body, bodies, t, 0.0, Derivative());
		b = evaluate(body, bodies, t, 0.5 * dt, a);
		c = evaluate(body, bodies, t, 0.5 * dt, b);
		d = evaluate(body, bodies, t, dt, c);
	
		Vec2 dposdt = (a.dpos + ( (b.dpos + c.dpos)  * 2) + d.dpos) * (1.0 / 6.0); 
		Vec2 dveldt = (a.dvel + ((b.dvel + c.dvel) * 2) + d.dvel) * (1.0 / 6.0);

		body.setPosition(body.getPosition() + (dposdt * dt));
		body.setVelocity(body.getVelocity() + (dveldt * dt));
	
		body.setForces(Vec2(0, 0));
	}

	// on prend initial ici en param�tre parce que le deuxi�me body est l'�tat avanc� dans le temps de l'initial, et n'est donc pas le m�me 
	//
	Vec2 accelerate(Body& initial, Body &b, std::vector<Body> &bodies, double dt){ 
		//retourne l'acc�l�ration selon l'�tat (position) et le temps

		//calcule aussi la gravit� de tous les autres corps
		//sur ce corps-ci, si ils sont assez proche
		Vec2 totalGravity;
		if (!initial.isFixed() && initial.isSolid()){ // si l'objet est affect� par la gravit�
			for (int i(0); i < bodies.size(); i++){
				if (&bodies[i] != &initial){ // si l'objet est diff�rent de lui-m�me
					//l'objet X est attir� par l'objet Y si et seulement si l'objet X est solide
					if (b.getPosition().getDist(bodies[i].getPosition())  <  1500 && !checkIfCollide(b, bodies[i], dt) ) { // si l'objet est assez proche ET qu'ils sont pas l'un dans l'autre
						totalGravity += gravity(b, bodies[i]);
					}
				}
			}
		}

		initial.setForces(initial.getForces() + totalGravity);
		b.setForces(initial.getForces());

		return (b.getForces()) / (b.getMass());
	}
}