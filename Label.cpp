#include "Label.h"

// CONSTRUCTOR
Label::Label(int _posX, int _posY, string _label)
{
	posX = _posX;
	posY = _posY;
	label = _label;
	hauteur = 30;
	largeur = 40;

	// Ajustement de la largeur pour acceuillir le label
	for (string::iterator i = label.begin(); i != label.end(); ++i)
	{
		char c = *i;

		// Augmentation de la largeur de 20 pixels par majuscule
		if (isupper(c)){ largeur += 20; } 

		// Augmentation de la largeur de 10 pixels par miniscule
		else{ largeur += 10; } 
	}
}

// DESTRUCTOR
Label::~Label(){}

// SETTERS
void Label::setLabel(string _label){label = _label;}

// GETTERS
string Label::getLabel(){ return label; }
int Label::getLargeur(){ return largeur; }


// METHODS
// Dessin d'un label
void Label::draw(int windowW, int windowH)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(windowW - posX, windowH - posY, 0);

	// Couleur du texte
	glColor3f(0,0,0);

	// Positionnement du début du texte
	glRasterPos2i(20, 8);

	// Choix Police
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;

	// Ecriture du texte
	for (string::iterator i = label.begin(); i != label.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}

	// Dessin du fond
	glBegin(GL_QUADS);
	glColor3f(0.9,0.9,0.9);
	glVertex2i(0, 0);
	glVertex2i(largeur, 0);
	glVertex2i(largeur, hauteur);
	glVertex2i(0, hauteur);
	glEnd();
}