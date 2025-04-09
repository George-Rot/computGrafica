
#ifdef WIN32
#include <windows.h>
#include "glut.h"
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

//#include "Ponto.h"

#ifndef LINHA_H
#define LINHA_H

#include "Ponto.h"

struct Linha {
    Ponto a, b, c;

    // Construtor padrão
    Linha();

    // Construtor com argumentos
    Linha(Ponto A, Ponto B, Ponto C);

	Ponto getA() const { return a; }
	Ponto getB() const { return b; }
	Ponto getC() const { return c; }

};

#endif // LINHA_H
