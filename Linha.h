
#ifdef WIN32
#include <windows.h>
#include "glut.h"
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

//#include "Ponto.h"
#ifndef LINHA_H
#define LINHA_H
#include "Ponto.h"

class Linha {
private:
    Ponto ponto1;
    Ponto ponto2;
    Ponto ponto3;

public:
    Linha(const Ponto& p1, const Ponto& p2, const Ponto& p3);
    Linha();

    // Getters
    Ponto getA() const;
    Ponto getB() const;
    Ponto getC() const;

    // Setters
    void setA(const Ponto& p);
    void setB(const Ponto& p);
    void setC(const Ponto& p);
};

#endif // LINHA_H