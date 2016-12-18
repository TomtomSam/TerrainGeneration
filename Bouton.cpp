#include "Bouton.h"

// CONSTRUCTOR
Bouton::Bouton(int _posX, int _posY, string _label) :IUobject(_posX, _posY, "Bouton")
{
	this->setLabel(_label);
	this->setHauteur(30);
	this->setLargeur(40);

	// Ajustement de la largeur pour acceuillir le label
	for (string::iterator i = _label.begin(); i != _label.end(); ++i)
	{
		char c = *i;

		// Augmentation de la largeur de 20 pixels par majuscule
		if (isupper(c)){ this->setLargeur(this->getLargeur() + 20); }

		// Augmentation de la largeur de 10 pixels par miniscule
		else{ this->setLargeur(this->getLargeur() + 10); }
	}
}

Bouton::~Bouton(){}


// METHODS
// Dessin d'un label
void Bouton::draw(int windowW,int windowH)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(windowW - this->getPosX(), windowH - this->getPosY(), 0);

	//Couleur du texte
	glColor3f(0,0,0);

	// Positionnement du texte
	glRasterPos2i(20,  8);

	// Choix Police
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;

	// Ecriture du texte
	int tailleLabel = this->getLabel().size();
	for (int i = 0; i < tailleLabel; i++)
	{
		char c = this->getLabel()[i];
		glutBitmapCharacter(font, c);
	}

	// Mise en valeur des bords en cas de survol
	if (this->getSurvol())
	{
		glLineWidth(4.0f);
		glBegin(GL_LINE_LOOP);
		glColor3f(0,0,0);
		glVertex2i(0, 0);
		glVertex2i(this->getLargeur(), 0);
		glVertex2i(this->getLargeur(), this->getHauteur());
		glVertex2i(0, this->getHauteur());
		glEnd();
		glLineWidth(1.0f);
	}

	// Dessin du fond
	glBegin(GL_QUADS);
	glColor3f(0.8,0.9,0.8);
	glVertex2i(0, 0);
	glVertex2i(this->getLargeur(), 0);
	glVertex2i(this->getLargeur(), this->getHauteur());
	glVertex2i(0, this->getHauteur());
	glEnd();
}

// Detection du survol du bouton par la souris
bool Bouton::isMouseInMe(int x, int y,int windowW,int windowH)
{
	if ((x<(windowW - this->getPosX() + this->getLargeur())) && (x>(windowW - this->getPosX())) 
		&& (y>(this->getPosY() - this->getHauteur())) && (y<this->getPosY()))
	{
		this->setSurvol(true);
	}
	else
	{
		this->setSurvol(false);
	}

	return this->getSurvol();
}