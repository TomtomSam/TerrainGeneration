#define _USE_MATH_DEFINES
#include "FreeFlyCamera.h"

// CONSTRUCTOR
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
	focale = 70.0f;
	close = 0.5f;
	distant = 2048.f;

	// Initialisation de  la position de la camera
	camPos = Vector3D(camPosX,camPosY,camPosZ);

	// Initialisation des vecteurs 'view'
	forwardView = Vector3D(atPosX,atPosY,atPosZ);
	upWorld = Vector3D(0, 1, 0);
	rightView = upWorld.crossProduct(forwardView);

	// Initialisation de la cible a partir de la camera et du vecteur vision
	targetPos = camPos + forwardView;

	// Initialisation des angles de rotation (coordonnees spheriques) pour calculer le vecteur vision de la camera
	angleTheta = static_cast<float>(0.5*M_PI);
	anglePhi = static_cast<float>(0.3*M_PI);
}

// DESTRUCTOR
FreeFlyCamera::~FreeFlyCamera()
{
}

// GETTERS
Vector3D FreeFlyCamera::getcamPos(){return camPos;}
Vector3D FreeFlyCamera::gettargetPos(){ return targetPos; }
Vector3D FreeFlyCamera::getupWorld(){ return upWorld; }

// SETTERS
void FreeFlyCamera::setMoveSensitivity(float MoveSensitivity){ moveSensitivity = MoveSensitivity; }
void FreeFlyCamera::setMouseSensitivity(float MouseSensitivity){ mouseRotSensitivity = MouseSensitivity; }
void FreeFlyCamera::setFar(float _far){ distant = _far; }
float FreeFlyCamera::getFar(){return distant;}
float FreeFlyCamera::getNear(){return close;}
float FreeFlyCamera::getFocale(){return focale;}

void FreeFlyCamera::resetParam()
{
	//Reset des param�tres de la cam�ra pour correspondre � la configuration constructeur
	deltaTheta = 0.0f;
	deltaPhi = 0.0f;
	deltaMove = 0.0f;
	deltaStrafe = 0.0f;
	xOrigin = -1;
	yOrigin = -1;

	forwardView = targetPos;
	rightView = upWorld.crossProduct(forwardView);
	targetPos = camPos + forwardView;
	angleTheta = static_cast<float>(0.5*M_PI);
	anglePhi = static_cast<float>(0.3*M_PI);
}


void FreeFlyCamera::setcamPos(float camPosX, float camPosY, float camPosZ)
{
	camPos.setVx(camPosX);
	camPos.setVy(camPosY);
	camPos.setVz(camPosZ);
}
void FreeFlyCamera::settargetPos(float atPosX, float atPosY, float atPosZ)
{
	targetPos.setVx(atPosX);
	targetPos.setVy(atPosY);
	targetPos.setVz(atPosZ);
}

// METHODS
// Fonction d'incrementation du mouvement de la camera
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

// Fonction de remise a zero de l'avance/recul de la camera
void FreeFlyCamera::resetDeltaMove(){ deltaMove = 0.0f; }

// Fonction de remise a zero du strafe de la camera
void FreeFlyCamera::resetDeltaStrafe(){ deltaStrafe = 0.0f; }

// Fonction de calcul des angles de rotation de la camera lorsque le clic gauche de la souris est enfonce
void FreeFlyCamera::setBouttonUp()
{
	angleTheta += deltaTheta;
	anglePhi += deltaPhi;
	xOrigin = -1;
	yOrigin = -1;
}

// Fonction de stockage des coordonnees de la souris au moment d'un clic gauche
void FreeFlyCamera::setBouttonDown(int x, int y)
{
	xOrigin = x;
	yOrigin = y;
}

// Fonction de gestion du deplacement de la camera
void FreeFlyCamera::cameraMovement() 
{

	camPos = camPos + deltaStrafe*rightView + deltaMove*forwardView;
	// Mise a jour de la cible
	targetPos = camPos + forwardView;
}

// Fonction de mise a jour des reglages de la camera
bool FreeFlyCamera::ActualiserCamera(int x, int y)
{
	if (xOrigin >= 0 || yOrigin >= 0) 
	{
		// Mise a jour des deltas des angles theta et phi
		deltaTheta = (x - xOrigin)*mouseRotSensitivity;
		deltaPhi = -(y - yOrigin)*mouseRotSensitivity;

		// Calcul du nouveau vecteur vision :
		forwardView.setVx(sin(anglePhi + deltaPhi)*cos(angleTheta + deltaTheta));
		forwardView.setVy(-cos(anglePhi + deltaPhi));
		forwardView.setVz(sin(anglePhi + deltaPhi)*sin(angleTheta + deltaTheta));

		// Normalisation du vecteur forward
		forwardView = forwardView.normalize();

		// Mise a jour de rightView (le cross product entre up et forward)
		rightView = upWorld.crossProduct(forwardView);

		// Mise a jour la cible (point "vise" par la camera)
		targetPos = camPos + forwardView;

		return true;
	}

	return false;
}