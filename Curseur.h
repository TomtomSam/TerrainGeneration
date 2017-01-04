#ifndef CURSEUR_H
#define CURSEUR_H

#include "IUobject.h"
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

class Curseur : public IUobject
{

private:
	int posCurseurX;
	float valeur;
	int largeurCurseur;
	int hauteurCurseur;

public:
	Curseur(int _posX,int _posY);
	~Curseur();

	
	int getPosCurseur();
	float getValue();
	void setPosCurseur(int newPos);
	void setValue(float _value);
	
	void draw(int windowW, int windowH);
	bool isMouseInMe(int x, int y, int windowW, int windowH);
};

#endif // CURSEUR_H