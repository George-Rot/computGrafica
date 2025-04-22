// **********************************************************************
// PUCRS/Escola Politecnica
// COMPUTACAO GRAFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************
 
// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//
//  Para rodar em um terminal Windows, digite
//           mingw32-make -f Makefile.mk 
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
 

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>    
#endif

#include "Ponto.h"
#include "Poligono.h"

#include "Temporizador.h"

#include "ListaDeCoresRGB.h"

#include "Linha.h"
#include "Bezier.h"

// Limites logicos da area de desenho
Ponto Min, Max;

Poligono Poly;

bool desenha = false;

float angulo=0.0;

Ponto PosicaoDoCampoDeVisao, PontoClicado;
Ponto novoPonto[3];
int indexPonto = 0;
Linha linhas[20];
Bezier curvas[20];
bool poligControle = true;

int qtdLinhas = 0;
int modo = 0;

bool FoiClicado = false;


// **********************************************************************
//
// **********************************************************************
void init()
{
    
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Min = Ponto (-20, -20);
    Max = Ponto (20, 20);
    
    Poly.LePoligono("Retangulo1x1.txt");
string modoTexto;
            switch (modo) {
                case 0:
                    modoTexto = "Modo Criar Curvas";
                    break;
                case 1:
                    modoTexto = "Modo Sequencial de Curvas";
                    break;
                case 2:
                    modoTexto = "Modo Conectar com Linha Existente";
                    break;
                case 3:
                    modoTexto = "Modo Deletar Ponto";
                    break;
                default:
                    modoTexto = "Modo Desconhecido";
                    break;
            }
            glColor3f(1.0, 1.0, 1.0); // Define a cor do texto (branco)
            glRasterPos2f(Min.x + 1, Max.y - 1); // Define a posição do texto na janela
            for (char c : modoTexto) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Renderiza o texto
            }
}

double nFrames=0;
double TempoTotal=0;
Temporizador T;
double AccumDeltaT=0;

// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;
    
    if (AccumDeltaT > 1.0/30) // fixa a atualizacao da tela em 30
    {
        AccumDeltaT = 0;
        angulo+=1.0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    
    glOrtho(Min.x,Max.x, Min.y,Max.y, 0,1);
    //glOrtho(0,10, 0,10, 0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// **********************************************************************
//
// **********************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}
// **********************************************************************
// void DesenhaTriangulo()
// **********************************************************************
void DesenhaTriangulo(Ponto A, Ponto B, Ponto C)
{
    glBegin(GL_TRIANGLES);
        glVertex2f(A.x, A.y);
        glVertex2f(B.x, B.y);
        glVertex2f(C.x, C.y);
    glEnd();
}
// **********************************************************************
void RotacionaAoRedorDeUmPonto(float alfa, Ponto P)
{
    glTranslatef(P.x, P.y, P.z);
    glRotatef(alfa, 0,0,1);
    glTranslatef(-P.x, -P.y, -P.z);
}
//
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{

	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    // Define os limites logicos da area OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if(FoiClicado){
        switch(modo){
            case 0://modo criar curvas
                cout << "Modo Criar Curvas" << endl;
                cout << "Ponto Clicado: " << PontoClicado.x << " " << PontoClicado.y << endl;
                if(indexPonto < 3){
                    novoPonto[indexPonto] = PontoClicado;
                    indexPonto++;
                }
                if(indexPonto == 3){
                    Ponto a, b, c;
                    a = novoPonto[0];
                    b = novoPonto[1];
                    c = novoPonto[2];
                    linhas[qtdLinhas] = Linha(a, b, c);
                    Bezier Bezier(novoPonto[0], novoPonto[1], novoPonto[2]); //cria a curva dos respectivos pontos
                    curvas[qtdLinhas] = Bezier; // armzena a curva
                    Bezier.Traca();
                    Bezier.TracaPoligonoDeControle();
                    qtdLinhas++;
                    indexPonto = 0;
                }

            break;
            case 1://modo sequencial de curvas
                if(qtdLinhas == 0){
                    cout << "Não há curvas a serem ligadas" << endl;
                    break;
                }
                if(indexPonto < 2){
                    novoPonto[indexPonto] = PontoClicado;
                    indexPonto++;
                }
                if(indexPonto == 2){
                    Ponto A = linhas[qtdLinhas-1].getC();

                    linhas[qtdLinhas] = Linha(A, novoPonto[0], novoPonto[1]);
                    Bezier Bezier(A, novoPonto[0], novoPonto[1]); //cria a curva dos respectivos pontos
                    curvas[qtdLinhas] = Bezier; // armzena a curva
                    Bezier.Traca();
                    Bezier.TracaPoligonoDeControle();
                    qtdLinhas++;
                    indexPonto = 0;
                }
            
            break;
            case 2: // continuidade de derivada
                if (indexPonto == 0) {
                    bool validStartPoint = false;
                    for (int i = 0; i < qtdLinhas; i++) {
                        Ponto start = linhas[i].getA();
                        Ponto end = linhas[i].getC();

                        if (abs(PontoClicado.x - start.x) <= 20 && abs(PontoClicado.y - start.y) <= 20) {
                            novoPonto[0] = start;
                            validStartPoint = true;
                            break;
                        } else if (abs(PontoClicado.x - end.x) <= 20 && abs(PontoClicado.y - end.y) <= 20) {
                            novoPonto[0] = end;
                            validStartPoint = true;
                            break;
                        }
                    }

                    if (!validStartPoint) {
                        cout << "Give a valid start point" << endl;
                        break;
                    }

                    indexPonto++;
                } else if (indexPonto < 3) {
                    novoPonto[indexPonto] = PontoClicado;
                    indexPonto++;
                }

                if (indexPonto == 3) {
                    // Ponto final da última curva
                    Ponto A = linhas[qtdLinhas - 1].getC();

                    // Ponto intermediário da última curva
                    Ponto B = linhas[qtdLinhas - 1].getB();

                    // Calcula o ponto inicial da nova curva para manter continuidade de derivada
                    Ponto D = A + (A - B); // Reflete o ponto intermediário em relação ao ponto final

                    // Cria a nova linha e curva com continuidade de derivada
                    linhas[qtdLinhas] = Linha(A, D, novoPonto[1]);
                    Bezier Bezier(A, D, novoPonto[1]); // Cria a curva dos respectivos pontos
                    curvas[qtdLinhas] = Bezier;        // Armazena a curva
                    Bezier.Traca();
                    Bezier.TracaPoligonoDeControle();
                    qtdLinhas++;
                    indexPonto = 0;
                }
                break;
                case 3: // deletar um ponto
                    // Deletar uma linha caso o clique ocorra em seu ponto inicial, médio ou final
                    for (int i = 0; i < qtdLinhas; i++) {
                        Ponto start = linhas[i].getA();
                        Ponto middle = linhas[i].getB();
                        Ponto end = linhas[i].getC();

                        // Calcula a hitbox com base no tamanho da janela
                        GLint viewport[4];
                        glGetIntegerv(GL_VIEWPORT, viewport);
                        float hitboxSize = (Max.x - Min.x) / viewport[2] * 10; // Ajusta o tamanho da hitbox

                        // Verifica se o clique está dentro da hitbox de algum dos pontos
                        if ((abs(PontoClicado.x - start.x) <= hitboxSize && abs(PontoClicado.y - start.y) <= hitboxSize) ||
                            (abs(PontoClicado.x - middle.x) <= hitboxSize && abs(PontoClicado.y - middle.y) <= hitboxSize) ||
                            (abs(PontoClicado.x - end.x) <= hitboxSize && abs(PontoClicado.y - end.y) <= hitboxSize)) {
                            
                            // Remove a linha e curva correspondente
                            for (int j = i; j < qtdLinhas - 1; j++) {
                                linhas[j] = linhas[j + 1];
                                curvas[j] = curvas[j + 1];
                            }
                            qtdLinhas--;
                            cout << "Linha deletada!" << endl;
                            break;
                        }
                    }
                    break;
            
            
        }
    }

        // Desenha os eixos para referência
        DesenhaEixos();

        // Redesenha todas as linhas e curvas armazenadas
        for (int i = 0; i < qtdLinhas; i++) {
            curvas[i].Traca(); // Desenha a curva
            if (poligControle) {
                curvas[i].TracaPoligonoDeControle(); // Desenha o polígono de controle, se habilitado
            }
        }
        // Escreve o modo atual no canto superior esquerdo da tela
        string modoTexto;
        switch (modo) {
            case 0:
                modoTexto = "Modo Criar Curvas";
                break;
            case 1:
                modoTexto = "Modo Sequencial de Curvas";
                break;
            case 2:
                modoTexto = "Modo Conectar com Linha Existente";
                break;
            case 3:
                modoTexto = "Modo Deletar Ponto";
                break;
            default:
                modoTexto = "Modo Desconhecido";
                break;
        }
        glColor3f(1.0, 1.0, 1.0); // Define a cor do texto (branco)
        glRasterPos2f(Min.x + 1, Max.y - 1); // Define a posição do texto na janela
        for (char c : modoTexto) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Renderiza o texto
        }
    
    FoiClicado = false;
    /*defineCor(GreenYellow);
    Ponto P1, P2, P3;
    P1 = Ponto(-10,-10);
    P2 = Ponto(0, 10);
    P3 = Ponto(10, -10);

    glPushMatrix();
        DesenhaTriangulo(P1, P2, P3);
    glPopMatrix();
        
    glColor3f(1,1,1); // R, G, B  [0..1]
    DesenhaEixos();
    
    defineCor(IndianRed);
    Poly.desenhaPoligono();
    */
    
	glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo n�mero de segundos e informa quanto frames
// se passaram neste per�odo.
// **********************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}

// **********************************************************************
// Esta fun��o captura o clique do botao direito do mouse sobre a �rea de
// desenho e converte a coordenada para o sistema de refer�ncia definido
// na glOrtho (ver fun��o reshape)
// Este c�digo � baseado em http://hamala.se/forums/viewtopic.php?t=20
// **********************************************************************
void Mouse(int button,int state,int x,int y)
{
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;
    GLdouble ox=0.0,oy=0.0,oz=0.0;

    if(state!=GLUT_DOWN)
      return;
    if(button!=GLUT_LEFT_BUTTON)
     return;
    cout << "Botao da direita! ";

    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;string modoTexto;
            switch (modo) {
                case 0:
                    modoTexto = "Modo Criar Curvas";
                    break;
                case 1:
                    modoTexto = "Modo Sequencial de Curvas";
                    break;
                case 2:
                    modoTexto = "Modo Conectar com Linha Existente";
                    break;
                case 3:
                    modoTexto = "Modo Deletar Ponto";
                    break;
                default:
                    modoTexto = "Modo Desconhecido";
                    break;
            }
            glColor3f(1.0, 1.0, 1.0); // Define a cor do texto (branco)
            glRasterPos2f(Min.x + 1, Max.y - 1); // Define a posição do texto na janela
            for (char c : modoTexto) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c); // Renderiza o texto
            }
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
    PontoClicado = Ponto(ox,oy,oz);
    FoiClicado = true;
}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
        case ' ':
            desenha = !desenha;
        break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
        case GLUT_KEY_UP:       // Se pressionar UP
            glutFullScreen ( ); // Vai para Full Screen
            break;
        case GLUT_KEY_RIGHT:       // Se pressionar RIGHT
            modo++;
            if(modo == 4)
                modo = 0;
            break;
        case GLUT_KEY_LEFT:       // Se pressionar LEFT
            modo--;
            if(modo < 0)
                modo = 3;
            break;
        case GLUT_KEY_DOWN:     // Se pressionar DOWN
                                // Reposiciona a janela
            glutPositionWindow (50,50);
            glutReshapeWindow ( 700, 500 );
            break;
        case 'p': // Se pressionar 'p'
            if(poligControle){
                poligControle = false;
                glClear(GL_COLOR_BUFFER_BIT); // Limpa o display
                for (int i = 0; i < qtdLinhas; i++) {
                    curvas[i].Traca(); // Desenha as curvas
                }
                glutSwapBuffers(); // Atualiza o display
            }else{
                poligControle = true;
                glClear(GL_COLOR_BUFFER_BIT); // Limpa o display
                for (int i = 0; i < qtdLinhas; i++) {
                    curvas[i].TracaPoligonoDeControle(); // Desenha os polígonos de controle
                }
                for (int i = 0; i < qtdLinhas; i++) {
                    curvas[i].Traca(); // Desenha as curvas
                }
                glutSwapBuffers(); // Atualiza o display
            }
            break;
        default:
            break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;
    system ("pwd");

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 500, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow    ( "Primeiro Programa em OpenGL" );

    // executa algumas inicializa��es
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser� chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida��o da tela. A funcao "display"
    // ser� chamada automaticamente sempre que a
    // m�quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );
    glutMouseFunc(Mouse);

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
