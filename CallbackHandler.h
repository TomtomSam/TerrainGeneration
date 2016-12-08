#ifndef CALLBACKHANDLER_H
#define CALLBACKHANDLER_H

#include <stdlib.h>//mis en premier pour que l'on puisse charger l'exit de glut et non celui de stdlib dans le .cpp
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

#include "FreeFlyCamera.h"
#include "heightMap.h"
#include "VBO.h"

using namespace std;

GLvoid clavier(unsigned char touche, int x, int y);
GLvoid souris(int bouton, int etat, int x, int y);
GLvoid clavierUp(unsigned char key, int x, int y);
GLvoid deplacementSouris(int x, int y);
GLvoid redimensionner(int w, int h);
GLvoid releaseSpecialKey(int key, int x, int y);

#endif //CALLBACKHANDLER_H