#include "Label.h"


Label::Label(int _posX, int _posY, string _label)
{

	posX = _posX;
	posY = _posY;
	label = _label;
	hauteur = 30;

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


Label::~Label()
{
}


//SETTERS
void Label::setLabel(string _label){label = _label;}
string Label::getLabel(){ return label; }
int Label::getLargeur(){ return largeur; }


//METHODES
void Label::draw(int windowW, int windowH)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(windowW - posX, windowH - posY, 0);

	//Couleur du texte
	glColor3f(0.86f, 0.87f, 0.94f);
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

	

	//Dessin du fond
	glBegin(GL_QUADS);
	//glColor3f(0.52f, 0.25, 0.66);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(0, 0);
	glVertex2i(largeur, 0);
	glVertex2i(largeur, hauteur);
	glVertex2i(0, hauteur);
	glEnd();



}