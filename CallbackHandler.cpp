#include "CallbackHandler.h"

using namespace std;

extern FreeFlyCamera camera;
extern VBO monVBO;
extern heightMap maMap;
extern int windowW;
extern int windowH;

// Definition de la fonction gerant les interruptions clavier
GLvoid clavier(unsigned char touche, int x, int y) 
{
	switch (touche) {
	// Deplacement de la camera vers la gauche
	case 'q':
	case 'Q':
		camera.incrementMouvement("deltaStrafe", '+');
		break;
	// Deplacement de la camera vers la droite
	case 'd':
	case 'D':
		camera.incrementMouvement("deltaStrafe", '-');
		break;
	// Deplacement de la camera vers l'avant
	case 'z':
	case 'Z':
		camera.incrementMouvement("deltaMove", '+');
		break;
	// Deplacement de la camera vers l'arriere
	case 's':
	case 'S':
		camera.incrementMouvement("deltaMove", '-');
		break;
	// Affichage des polygones remplis
	case 'p': 
	case 'P':
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	// Affichage en mode fil de fer
	case 'o': 
	case 'O':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	// Affichage en nuage de points
	case 'i': 
	case 'I':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
		/*case 'h': // shader activate
		case 'H':
		glUseProgram(prog.getProgramID());
		break;
		case 'y': // shader deactivate
		case 'Y':
		glUseProgram(0);
		break;*/ 
	// Rehaussage du seuil de l'ocean
	case '+': 
		maMap.setPosOcean(maMap.getPosOcean()+1);
		monVBO.FeedCol(maMap.getCol());
		monVBO.FeedPos(maMap.getPos());
		monVBO.ActualizeColBuffer();
		monVBO.ActualizePosBuffer();
		break;
	// Abaissement du seuil de l'ocean
	case '-':
		maMap.setPosOcean(maMap.getPosOcean() - 1);
		monVBO.FeedCol(maMap.getCol());
		monVBO.FeedPos(maMap.getPos());
		monVBO.ActualizeColBuffer();
		monVBO.ActualizePosBuffer();
		break;
	// Ecriture du fichier .obj associe a la map
	case 'w':
	case 'W':
		maMap.ecrireFichierObj();
		break;
	// Dilatation horizontale de la map
	case 'm':
	case 'M':
		maMap.setDilatation(maMap.getDilatation() + 0.1);	
		monVBO.FeedCol(maMap.getCol());
		monVBO.FeedPos(maMap.getPos());
		monVBO.ActualizeColBuffer();
		monVBO.ActualizePosBuffer();
		break;
	// Compression horizontale de la map
	case 'l':
	case 'L':
		maMap.setDilatation(maMap.getDilatation() - 0.1);	
		monVBO.FeedCol(maMap.getCol());
		monVBO.FeedPos(maMap.getPos());
		monVBO.ActualizeColBuffer();
		monVBO.ActualizePosBuffer();
		break;
	// Quitter l'application
	case 27:
		monVBO.DestroyVBO();
		exit(0);
		break;
	}

	// Reaffichage de la scene
	glutPostRedisplay();
}

// Fonction de rappel de la souris
GLvoid souris(int bouton, int etat, int x, int y)
{
	if (bouton == GLUT_LEFT_BUTTON) {
		if (etat == GLUT_UP) 
		{
			// Mise a jour des angles theta et phi
			camera.setBouttonUp();
		}
		else  
		{	
			// Stockage des positions de la souris
			camera.setBouttonDown(x,y);
		}
	}
}

// Fonction de gestion du deplacement de la souris
void deplacementSouris(int x, int y) 
{
	bool affiche = false;

	// Actualisation des vecteurs de la caméra
	affiche = camera.ActualiserCamera(x, y);

	// Reaffichage si le clic de la souris est actif
	if (affiche){ glutPostRedisplay();}
}

// Callback de redimensionnement de la fenêtre
GLvoid redimensionner(int _windowW, int _windowH)
{
	// Stockage des dimensions de la fenetres
	windowW = _windowW;
	windowH = _windowH;
	
	// Gestion des divisions par 0
	if(windowH==0)
		windowH = 1;

	// Choix de la matrice de projection
	glMatrixMode(GL_PROJECTION);

	// Reinitialisation de la matrice
	glLoadIdentity();

	//Projection Orthographique
	gluOrtho2D(0.0, windowW, 0.0, windowH);

	// Gestion du viewport
	glViewport(0, 0, windowW, windowH);

	// Projection Perspective
	gluPerspective(camera.getFocale(), 1.0, camera.getNear(), camera.getFar());

	// Retour a la pile modelview
	glMatrixMode(GL_MODELVIEW);
}

// Fonction de gestion du relachement d'une touche clavier
void clavierUp(unsigned char key, int x, int y) 
{
	switch (key) 
	{
	// Arret du strafe
	case 'q':
	case 'd':
	case 'Q':
	case 'D':
		camera.resetDeltaStrafe();
		break;

	// Arret de l'avance/recul
	case 'z':
	case 's':
	case 'Z':
	case 'S':
		camera.resetDeltaMove();
		break;
	}
}
