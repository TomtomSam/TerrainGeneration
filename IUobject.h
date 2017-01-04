#ifndef IUOBJECT_H
#define IUOBJECT_H

#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

class IUobject
{

private:
	int posX;
	int posY;
	int hauteur;
	int largeur;
	bool survol;
	string tag;
	string label;

public:
	bool getSurvol();
	int getLargeur();
	int getHauteur();
	int getPosX();
	int getPosY();
	string getTag();
	string getLabel();

	void setLargeur(int _largeur);
	void setHauteur(int _hauteur);
	void setSurvol(bool _survol);
	void setLabel(string _label);

	virtual void draw(int windowW, int windowH)=0;
	virtual bool isMouseInMe(int x, int y, int windowW, int windowH)=0;

	IUobject(int _posX,int _posY, string _tag);
	~IUobject();
};

# endif //IUOBJECT_H
