#ifndef LABEL_H
#define LABEL_H

#include "IUobject.h"
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <locale>

using namespace std;

class Label : public IUobject
{

private:

public:
	Label(int _posX, int _posY, string _label);
	~Label();
	
	virtual void draw(int windowW, int windowH);
	virtual bool isMouseInMe(int x, int y, int windowW, int windowH);
	
};

#endif // LABEL_H