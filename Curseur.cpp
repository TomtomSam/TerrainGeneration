#include "Curseur.h"


//Dynamic cast pour utiliser les fonction fille à partir d'un objet mère
	/*Curseur moncurseur;
	IUobject& r_IUobject = moncurseur;

	Curseur& r_curseur = dynamic_cast<Curseur&>(r_IUobject);*/
	
/*Déplacement du curseur
Quand on clique sur le curseur 
on active le booléen isCliked de l'interface utilisateur
en même temps, on a mis à jour l'index de l'objet survolé
on ajuste la position du curseur dans le call back de deplacment actif de la souris
la position pixel du curseur est bloquée entre W-posX et W-posX+largeur
la position relative du curseur est entre 0 et 200 par rapport à posX
on calcule la différence (W-posX)-x ça doit être compris entre -200 et 0 et posCurseur=-cette différence
*/

/*Mapping des valeur du curseur
La dilatation commence à 1
l'océan commence à PosOcean

Quand on clique sur un curseur l'attribut cliked est actif
Action du curseur Océan: 
si isCliked=true, changer la position de l'océan prévisualisé
si is clicked est faux, actualiser les couleurs

Action du curseur Dilatation:
si isCliked =true dilater l'océan prévisualisé
si isCliked=false actualiser pos buffer

*/

// CONTRSUCTOR
Curseur::Curseur(int _posX, int _posY) : IUobject(_posX,_posY,"Curseur")
{
	//Dimensions de la barre fixe du curseur
	this->setHauteur(2);
	this->setLargeur(208);

	//Dimensions de la partie mobile du curseur
	posCurseurX =  50;
	largeurCurseur=8;
	hauteurCurseur = 20;
}

// DESTRUCTOR
Curseur::~Curseur()
{
}

// GETTERS
int Curseur::getPosCurseur(){ return posCurseurX; }
float Curseur::getValue(){ return valeur; }

// SETTERS
void Curseur::setPosCurseur(int newPos){ posCurseurX = newPos; }
void Curseur::setValue(float _value){ valeur = _value; }

// METHODS
// Dessin d'un curseur
void Curseur::draw(int windowW, int windowH)
{
	// Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(windowW - this->getPosX(), windowH - this->getPosY(), 0);

	//Dessin de la jauge
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.9, 0.8);
	glVertex2i(0, 0);
	glVertex2i(200, 0);
	glVertex2i(200, 2);
	glVertex2i(0, 2);
	glEnd();

	glTranslatef(posCurseurX, -(hauteurCurseur-1)/2, 0);
	//Dessin du curseur
	glBegin(GL_QUADS);
	if (this->getSurvol()){ glColor3f(0.2, 0.3, 0.8); }
	else{ glColor3f(0.8, 0.9, 0.8); }
	glVertex2i(0, 0);
	glVertex2i(largeurCurseur, 0);
	glVertex2i(largeurCurseur, hauteurCurseur);
	glVertex2i(0, hauteurCurseur);
	glEnd();

}

// Detection du survol du curseur par la souris
bool Curseur::isMouseInMe(int x, int y, int windowW, int windowH)
{
	//Detection de la souris dans la partie mobile du curseur
	if ((x<(windowW - this->getPosX() + posCurseurX+largeurCurseur)) && (x>(windowW - this->getPosX()+posCurseurX))
		&& (y>(this->getPosY() - hauteurCurseur/2)) && (y<this->getPosY()+hauteurCurseur/2))
	{
		this->setSurvol(true);
	}
	else
	{
		this->setSurvol(false);
	}

	return this->getSurvol();
}