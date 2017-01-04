#include "Label.h"

// CONSTRUCTOR
Label::Label(int _posX, int _posY, string _label) : IUobject(_posX, _posY, "Label")
{
	this->setLabel(_label);
	this->setHauteur(30); 
	this->setLargeur( 40);

	// Ajustement de la largeur pour acceuillir le label
	for (string::iterator i = _label.begin(); i != _label.end(); ++i)
	{
		char c = *i;

		// Augmentation de la largeur de 20 pixels par majuscule
		if (isupper(c)){ this->setLargeur(this->getLargeur()+20); } 

		// Augmentation de la largeur de 10 pixels par miniscule
		else{ this->setLargeur(this->getLargeur() + 10); }
	}
}

// DESTRUCTOR
Label::~Label(){}

// METHODS
// Dessin d'un label
void Label::draw(int windowW, int windowH)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(windowW - this->getPosX(), windowH - this->getPosY(), 0);

	// Couleur du texte
	glColor3f(0,0,0);

	// Positionnement du début du texte
	glRasterPos2i(20, 8);

	// Choix Police
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;

	// Ecriture du texte
	int tailleLabel = this->getLabel().size();
	for (int i = 0; i < tailleLabel; i++)
	{
		char c = this->getLabel()[i];
		glutBitmapCharacter(font, c);
	}

	// Dessin du fond
	glBegin(GL_QUADS);
	glColor3f(0.9,0.9,0.9);
	glVertex2i(0, 0);
	glVertex2i(this->getLargeur(), 0);
	glVertex2i(this->getLargeur(), this->getHauteur());
	glVertex2i(0, this->getHauteur());
	glEnd();
}

// Detection du survol du label par la souris
bool Label::isMouseInMe(int x, int y, int windowW, int windowH){return false;}