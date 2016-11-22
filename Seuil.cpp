#include "Seuil.h"

Seuil::Seuil(void)
{
}


Seuil::~Seuil(void)
{
}

void Seuil::setHauteur(float _hauteur)
{
	hauteur = _hauteur;
}

float Seuil::getHauteur()
{
	return hauteur;
}

Seuil Seuil::operator++(int)
{
   Seuil temp = *this;
   temp.setHauteur(temp.getHauteur()+1);
   return temp;
}

Seuil Seuil::operator--(int)
{
   Seuil temp = *this;
   temp.setHauteur(temp.getHauteur()-1);
   return temp;
}