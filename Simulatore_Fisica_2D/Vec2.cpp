// vec2.cpp  -  SORGENTE: dice COME funzionano le cose dichiarate nell'header.
#include "Vec2.h"
#include <cmath>   // per std::sqrt

// "Vec2::" dice al compilatore che questa funzione e' il metodo length() di Vec2.
// Dentro il metodo posso usare x e y direttamente.
double Vec2::length() const {
	return std::sqrt(x * x + y * y);   // teorema di Pitagora
}

// const& : niente copia (veloce) e il compilatore mi impedisce di modificare a o b.
double dot(const Vec2& a, const Vec2& b) {
	return a.x * b.x + a.y * b.y;      // prodotto scalare
}

Vec2 operator+(const Vec2& a, const Vec2& b) {
	return { a.x + b.x, a.y + b.y };   // ritorno un nuovo Vec2
}

Vec2 operator-(const Vec2& a, const Vec2& b) {
	return { a.x - b.x, a.y - b.y };   // ritorno un nuovo Vec2
}

Vec2 operator*(const Vec2& v, double k) {
	return { v.x * k, v.y * k };       // ritorno un nuovo Vec2
}

Vec2 operator*(double k, const Vec2& v) {
	return { v.x * k, v.y * k };       // ritorno un nuovo Vec2
}

Vec2& operator+=(Vec2& a, const Vec2& b) {
	a.x += b.x;
	a.y += b.y;
	return a;                          // ritorno l'oggetto modificato
}

Vec2& operator-=(Vec2& a, const Vec2& b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;                          // ritorno l'oggetto modificato
}

Vec2 operator/(const Vec2& v, double k) {
	return { v.x / k, v.y / k };       // ritorno un nuovo Vec2
}

double Vec2::lengthSquared() const {
	return x * x + y * y;      // lunghezza al quadrato, senza sqrt
}