#include "FreeFlyCamera.h"
#include <string>
#include <cmath>

FreeFlyCamera::FreeFlyCamera(float _moveSensitivity, float camPosX, float camPosY, float camPosZ, float atPosX, float atPosY, float atPosZ)
{

	float PI = 3.141592;

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
	angleTheta = 0.5*PI;
	anglePhi = 0.3*PI;
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