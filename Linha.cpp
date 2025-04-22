

#include <cstdlib>

#include "Linha.h"
#include "Ponto.h"

// Constructor with parameters
Linha::Linha(const Ponto& p1, const Ponto& p2, const Ponto& p3)
    : ponto1(p1), ponto2(p2), ponto3(p3) {}

// Default constructor
Linha::Linha() : ponto1(Ponto()), ponto2(Ponto()), ponto3(Ponto()) {}

// Getters
Ponto Linha::getA() const { return ponto1; }
Ponto Linha::getB() const { return ponto2; }
Ponto Linha::getC() const { return ponto3; }

// Setters
void Linha::setA(const Ponto& p) { ponto1 = p; }
void Linha::setB(const Ponto& p) { ponto2 = p; }
void Linha::setC(const Ponto& p) { ponto3 = p; }
