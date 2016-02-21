#pragma once

#include <math.h>

/**************************************
Classe qui représente un vecteur 2D
manipulable, sur lequel on peut 
effectuer des opérations de base
**************************************/

class Vec2{
public:
	Vec2();
	Vec2(const Vec2& v);
	Vec2(double, double);

	Vec2 operator+(const Vec2 &vec); 
	Vec2 operator-(const Vec2 &vec); //plus simple que de faire A + (-1 * b)

	Vec2 operator*(const double k); // multiplication par un scalaire
	double operator*(const Vec2 &vec); // produit scalaire ou dot product

	Vec2 operator/(const double k); // division par un scalaire, plus simple que d'écrire * 1 / k
	double crossProduct(const Vec2 &vec);

	Vec2& operator=(const Vec2 &vec);
	void operator+=(const Vec2 &vec);
	void operator*=(const double d);

	bool operator==(const Vec2 &vec);
	bool operator!=(const Vec2 &vec);

	Vec2 getNormalized();
	Vec2 getNormal(); // retourne la normale

	void normalize();

	double x() { 
		return m_x; 
	}

	double y() {
		return m_y; 
	}

	void setCoords(const double x, const double y) {
		m_x = x;
		m_y = y;
	}

	void setX(const double x) {
		m_x = x; 
	}

	void setY(const double y) { 
		m_y = y; 
	}

	double getLength(){ // retourne la longueur du vecteur
		return sqrt((m_x*m_x) + (m_y*m_y)); 
	} 

	double getDist(Vec2 vec) { // retourne la distance à un point (représenté par un vecteur)
		return sqrt(((m_x - vec.m_x) * (m_x - vec.m_x)) + ((m_y - vec.m_y) * (m_y - vec.m_y)));
	};

protected:
	double m_x;
	double m_y;
};