#ifndef FREEFLYCAMERA_H
#define FREEFLYCAMERA_H

#include "vector3d.h"
#include <string>
using namespace std;

class FreeFlyCamera
{

private:
	// angles de rotation (coordonnees spheriques) pour calculer le vecteur vision de la camera
	float angleTheta;
	float anglePhi;

	// Vecteurs de direction tout doit et à droite
	Vector3D forwardMove;
	Vector3D rightMove;

	// Direction de la vue
	Vector3D forwardView;
	Vector3D rightView;

	// Camera position
	Vector3D camPos;
	Vector3D targetPos;
	Vector3D upWorld;

	// les increments des angles theta et phi
	// egales a 0 quand on n'appuie sur aucune touche
	float deltaTheta;
	float deltaPhi;

	// increments de deplacement
	// remis a 0 quand on appuis sur rien
	float deltaMove;
	float deltaStrafe;

	// coordonnes de la souris au moment du clic gauche
	int xOrigin;
	int yOrigin;

	// Sensibilite de deplacement
	float moveSensitivity;
	float mouseRotSensitivity;

	// Near and Far Plane
	float close;
	float distant;

	// Focale
	float focale;

public:
	void setMoveSensitivity(float MoveSensitivity);
	void setMouseSensitivity(float MouseSensitivity);
	void setFar(float _far);
	float getFar();
	float getNear();
	float getFocale();

	Vector3D getcamPos();
	Vector3D gettargetPos();
	Vector3D getupWorld();

	void resetDeltaMove();
	void resetDeltaStrafe();
	void incrementMouvement(string nom,char signe);//+ ou - moveSensitivity
	void setBouttonUp();
	void setBouttonDown(int x, int y);

	void cameraMovement();
	bool ActualiserCamera(int x, int y);

	FreeFlyCamera(float _moveSensitivity, float camPosX, float camPosY, float camPosZ, float atPosX, float atPosY, float atPosZ);
	~FreeFlyCamera();
};

#endif // FREEFLYCAMERA_H
