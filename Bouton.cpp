#include "Bouton.h"


Bouton::Bouton(int _posX, int _posY, string _label)
{
	posX = _posX;
	posY = _posY;
	hauteur = 30;
	label = _label;
	survol = false;

	//Marge pour le Label
	largeur = 40;
	
	//Ajustement de la largeur pour acceuillir le label
	//20 pixels pour une majuscule et 10 pour une minuscule
	for (string::iterator i = label.begin(); i != label.end(); ++i)
	{
		char c = *i;
		if (isupper(c)){ largeur += 20; }
		else{ largeur += 10; }
	}

	
	
}


Bouton::~Bouton()
{
}


bool Bouton::getSurvol(){ return survol; }
int Bouton::getLargeur(){ return largeur; }

void Bouton::draw(int windowW,int windowH)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(windowW-posX, windowH-posY, 0);

	//Couleur du texte
	glColor3f(0,0,0);
	// Positionnement du texte
	glRasterPos2i(20,  8);
	// Choix Police
	void * font = GLUT_BITMAP_TIMES_ROMAN_24;
	
	// Ecriture du texte
	for (string::iterator i = label.begin(); i != label.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}

	//Mise en valeur des bords en cas de survol
	if (survol)
	{
		glLineWidth(4.0f);
		glBegin(GL_LINE_LOOP);
		glColor3f(0,0,0);
		glVertex2i(0, 0);
		glVertex2i(largeur, 0);
		glVertex2i(largeur, hauteur);
		glVertex2i(0, hauteur);
		glEnd();
		glLineWidth(1.0f);
	}

	//Dessin du fond
	glBegin(GL_QUADS);
	glColor3f(0.8,0.9,0.8);
	glVertex2i(0, 0);
	glVertex2i(largeur, 0);
	glVertex2i(largeur, hauteur);
	glVertex2i(0, hauteur);
	glEnd();

	

}

bool Bouton::isMouseInMe(int x, int y,int windowW,int windowH)
{
	if (x < (windowW - posX+largeur) && x>(windowW-posX) && y>(posY-hauteur) && y <posY)
	{
		survol = true;
	}
	else
	{
		survol = false;
	}

	return survol;
}