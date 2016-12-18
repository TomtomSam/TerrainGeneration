#include "IUobject.h"




// GETTERS
bool IUobject::getSurvol(){ return survol; }
int IUobject::getLargeur(){ return largeur; }
int IUobject::getHauteur(){ return hauteur; }
int IUobject::getPosX(){ return posX; }
int IUobject::getPosY(){ return posY; }
string IUobject::getTag(){ return tag; }
string IUobject::getLabel(){ return label; }

void IUobject::setLargeur(int _largeur){ largeur = _largeur; }
void IUobject::setHauteur(int _hauteur){ hauteur = _hauteur; }
void IUobject::setSurvol(bool _survol){ survol = _survol; }
void IUobject::setLabel(string _label){  label=_label; }


IUobject::IUobject(int _posX, int _posY, string _tag)
{
	posX = _posX;
	posY = _posY;
	tag = _tag;
	survol = false;
}


IUobject::~IUobject()
{
}
