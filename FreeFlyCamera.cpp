#define _USE_MATH_DEFINES
#include "FreeFlyCamera.h"
#include <string>
#include <cmath>

FreeFlyCamera::FreeFlyCamera(float _moveSensitivity, float camPosX, float camPosY, float camPosZ, float atPosX, float atPosY, float atPosZ)
{

	//Initialisation
	deltaTheta = 0.0f;
	deltaPhi = 0.0f;
	deltaMove = 0.0f;
	deltaStrafe = 0.0f;
	xOrigin = -1;
	yOrigin = -1;
	mouseRotSensitivity = 0.005f;
	moveSensitivity = _moveSensitivity;


	// on initialise la position de la camera
	camPos = Vector3D(camPosX,camPosY,camPosZ);

	// on initialise les vecteurs 'view'
	forwardView = Vector3D(atPosX,atPosY,atPosZ);
	upWorld = Vector3D(0, 1, 0);
	rightView = upWorld.crossProduct(forwardView);

	// Pour le FPS mode
	forwardMove = forwardView;
	rightMove = rightView;

	// on initialise la cible a partir de la camera et du vecteur vision
	targetPos = camPos + forwardView;

	// angles de rotation (coordonnees spheriques) pour calculer le vecteur vision de la camera
	angleTheta = static_cast<float>(0.5*M_PI);
	anglePhi = static_cast<float>(0.3*M_PI);
}


FreeFlyCamera::~FreeFlyCamera()
{
}

Vector3D FreeFlyCamera::getcamPos(){return camPos;}
Vector3D FreeFlyCamera::gettargetPos(){ return targetPos; }
Vector3D FreeFlyCamera::getupWorld(){ return upWorld; }

void FreeFlyCamera::setMoveSensitivity(float MoveSensitivity){ moveSensitivity = MoveSensitivity; }
void FreeFlyCamera::setMouseSensitivity(float MouseSensitivity){ mouseRotSensitivity = MouseSensitivity; }

void FreeFlyCamera::incrementMouvement(string nom, char signe)
{
	if (nom == "deltaMove")
	{
		if (signe == '+'){ deltaMove += moveSensitivity; }
		else { deltaMove -= moveSensitivity; }
	}
	else 
	{
		if (signe == '+'){ deltaStrafe += moveSensitivity; }
		else { deltaStrafe -= moveSensitivity; }
	}
}

void FreeFlyCamera::resetDeltaMove(){ deltaMove = 0.0f; }
void FreeFlyCamera::resetDeltaStrafe(){ deltaStrafe = 0.0f; }

void FreeFlyCamera::setBouttonUp()
{
	angleTheta += deltaTheta;
	anglePhi += deltaPhi;
	xOrigin = -1;
	yOrigin = -1;
}
void FreeFlyCamera::setBouttonDown(int x, int y)
{
	xOrigin = x;
	yOrigin = y;
}


// Fonction de gestion du deplacement de la camera
void FreeFlyCamera::cameraMovement() 
{

	camPos = camPos + deltaStrafe*rightView + deltaMove*forwardView;
	// Mettre a jour la cible
	targetPos = camPos + forwardView;
}

bool FreeFlyCamera::ActualiserCamera(int x, int y)
{
	if (xOrigin >= 0 || yOrigin >= 0) 
	{

		// mise a jour des deltas des angles theta et phi
		deltaTheta = (x - xOrigin)*mouseRotSensitivity;
		deltaPhi = -(y - yOrigin)*mouseRotSensitivity;

		// Calcul du nouveau vecteur vision :
		forwardView.setVx(sin(anglePhi + deltaPhi)*cos(angleTheta + deltaTheta));
		forwardView.setVy(-cos(anglePhi + deltaPhi));
		forwardView.setVz(sin(anglePhi + deltaPhi)*sin(angleTheta + deltaTheta));

		// normalisation du vecteur forward
		forwardView = forwardView.normalize();

		// Up ne change pas
		// et right est le cross product entre up et forward
		rightView = upWorld.crossProduct(forwardView);


		// Mettre a jour la cible = point "vise" par la camera
		targetPos = camPos + forwardView;

		return true;

	}

	return false;
}


//A METTRE DANS LE MAIN POUR FAIRE FONCTIONNER LA CAMERA

/*
// Definition de la fonction gerant les interruptions clavier
GLvoid clavier(unsigned char touche, int x, int y) {
	switch (touche) {
		// Q et D on strafe
	case 'q':
	case 'Q':
		cam.incrementMouvement("deltaStrafe", '+');
		break;
	case 'd':
	case 'D':
		cam.incrementMouvement("deltaStrafe", '-');
		break;
		// Z et S avance/recule
	case 'z':
	case 'Z':
		cam.incrementMouvement("deltaMove", '+');
		break;
	case 's':
	case 'S':
		cam.incrementMouvement("deltaMove", '-');
		break;
	case 27:
		exit(0);
		break;
	}

	// Reaffichage de la scene
	glutPostRedisplay();
}



// Fonction de gestion du clavier (touche relachee)
void clavierUp(unsigned char key, int x, int y) {

	// Que faire quand une touche est relachee
	switch (key) {

		// On arrete de strafer
	case 'q':
	case 'd':
	case 'Q':
	case 'D':
		cam.resetDeltaStrafe();
		break;

		// On arrete d'avance/reculer
	case 'z':
	case 's':
	case 'Z':
	case 'S':
		cam.resetDeltaMove();
		break;
	}
}

// Fonction de gestion du clavier (touche speciale relachee)
void releaseSpecialKey(int key, int x, int y) {

	switch (key) {
		// On remet le delta deplacement a 0
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		cam.resetDeltaMove();
		break;

		// On remet le delta strafe a 0
	case GLUT_KEY_RIGHT:
	case GLUT_KEY_LEFT:
		cam.resetDeltaStrafe();
		break;
	}
}

// Fonction de rappel de la souris
GLvoid souris(int bouton, int etat, int x, int y)
{
	// On ne fait quelque chose que sur le bouton gauche de la souris
	if (bouton == GLUT_LEFT_BUTTON) {
		// si on relache le bouton on met a jour les angles theta et phi
		// et on dit que l'on a pas clique
		if (etat == GLUT_UP) 
		{
			cam.setBouttonUp();
		}
		else  
		{	// state = GLUT_DOWN
			// si l'on a clique sur le bouton gauche
			// on garde les positions de la souris au moment du clic gauche
			cam.setBouttonDown(x,y);
		}
	}
}



// Fonction de gestion du deplacement de la souris
void deplacementSouris(int x, int y) {

	bool affiche = false;

	//on actualise les vecteurs de la caméra
	affiche = cam.ActualiserCamera(x, y);

	//on n'affiche que si on est en train de cliquer
	if (affiche){ glutPostRedisplay();}
}
*/