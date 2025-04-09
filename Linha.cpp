

#include <cstdlib>

#include "Linha.h"
#include "Ponto.h"

struct Linha {
    Ponto a, b, c;

    // Construtor padrão
    Linha() : a(Ponto()), b(Ponto()), c(Ponto()) {}

    // Construtor com argumentos
    Linha(Ponto A, Ponto B, Ponto C) : a(A), b(B), c(C) {}
};