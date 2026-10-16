// vec2.hpp  -  HEADER: dice COSA esiste (dichiarazioni), non come funziona.
#pragma once  // evita che il compilatore includa questo file due volte

struct Vec2 {
    double x;
    double y;

    // Solo dichiarazione: il corpo e' in vec2.cpp.
    // 'const' finale = questo metodo NON modifica l'oggetto.
    double length() const;

    double lengthSquared() const;  // lunghezza al quadrato, senza sqrt

};

// --- Funzioni libere (non appartengono alla struct) ---

double dot(const Vec2& a, const Vec2& b);

// Operatori di overloading
Vec2 operator+(const Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a, const Vec2& b);
Vec2 operator*(const Vec2& v, double k);
Vec2& operator+=(Vec2& a, const Vec2& b);
Vec2& operator-=(Vec2& a, const Vec2& b);
Vec2 operator*(double k, const Vec2& v);
Vec2 operator/(const Vec2& v, double k);  // divisione per scalare