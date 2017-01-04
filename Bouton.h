#ifndef BOUTON_H
#define BOUTON_H

#include "IUobject.h"
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <locale>

using namespace std;

class Bouton : public IUobject
{
private:
	

public:
	Bouton(int _posX,int _posY, string _label);
	~Bouton();

	virtual void draw(int windowW, int windowH);
	virtual bool isMouseInMe(int x, int y, int windowW, int windowH);

};

#endif // BOUTON_H