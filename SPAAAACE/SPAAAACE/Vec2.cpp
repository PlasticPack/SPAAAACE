#include "Vec2.h"

Vec2::Vec2() : m_x(0), m_y(0){

}

Vec2::Vec2(double x, double y) : m_x(x), m_y(y){

}

Vec2::Vec2(const Vec2& vec) {
	if (this != &vec){
		m_x = vec.m_x;
		m_y = vec.m_y;
	}
}

double Vec2::crossProduct(const Vec2 &vec){

	return (m_x * vec.m_y) - (m_y * vec.m_x);
}
Vec2 Vec2::getNormalized(){

	Vec2 vec(m_x, m_y);
	vec.normalize();

	return vec;
}

Vec2 Vec2::getNormal(){
	return Vec2(-m_y, m_x);
}


Vec2 Vec2::operator+(const Vec2 &vec){
	Vec2 result;
	result.m_x = m_x + vec.m_x;
	result.m_y = m_y + vec.m_y;
	return result;
}

void Vec2::normalize(){
	//on réduit sa longueur à 1
	operator*=(1 / getLength());
}

bool Vec2::operator==(const Vec2 &vec){

	return (m_x == vec.m_x) && (m_y == vec.m_y);
}

bool Vec2::operator!=(const Vec2 &vec){

	return !(operator==(vec));
}

Vec2 Vec2::operator-(const Vec2 &vec){
	Vec2 result;
	result.m_x = m_x - vec.m_x;
	result.m_y = m_y - vec.m_y;
	return result;
}

Vec2 Vec2::operator*(double k){
	Vec2 result;
	result.m_x = m_x * k;
	result.m_y = m_y * k;
	return result;
}

Vec2 Vec2::operator/(double k){
	Vec2 result;
	result.m_x = m_x / k;
	result.m_y = m_y / k;
	return result;
}

double Vec2::operator*(const Vec2 &vec){  // produit scalaire
	double result = 0;
	result = (m_x * vec.m_x) + (m_y * vec.m_y);
	return result;
}

void Vec2::operator+=(const Vec2 &vec){
	m_x += vec.m_x;
	m_y += vec.m_y;
}

void Vec2::operator*=(const double d){
	m_x *= d;
	m_y *= d;
}

Vec2& Vec2::operator=(const Vec2 &vec){

	if (this != &vec){
		m_x = vec.m_x;
		m_y = vec.m_y;
	}

	return *this;
}
