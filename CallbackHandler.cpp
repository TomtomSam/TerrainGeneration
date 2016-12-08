#include "CallbackHandler.h"

using namespace std;

extern FreeFlyCamera camera;
extern VBO monVBO;
extern heightMap maMap;

// Definition de la fonction gerant les interruptions clavier
GLvoid clavier(unsigned char touche, int x, int y) {
	switch (touche) {
		// Q et D on strafe
	case 'q':
	case 'Q':
		camera.incrementMouvement("deltaStrafe", '+');
		break;
	case 'd':
	case 'D':
		camera.incrementMouvement("deltaStrafe", '-');
		break;
		// Z et S avance/recule
	case 'z':
	case 'Z':
		camera.incrementMouvement("deltaMove", '+');
		break;
	case 's':
	case 'S':
		camera.incrementMouvement("deltaMove", '-');
		break;
	case 'p': // carre plein
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'o': // fil de fer
	case 'O':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'i': // sommets du carre
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
	case '+': //changer le seuil de l'océan
		maMap.setPosOcean(maMap.getPosOcean()+1);
		monVBO.FeedCol(maMap.getCol());
		monVBO.ActualizeColBuffer();
		break;
	case '-':
		maMap.setPosOcean(maMap.getPosOcean() - 1);
		monVBO.FeedCol(maMap.getCol());
		monVBO.ActualizeColBuffer();
		break;
	case 'w':
	case 'W':
		maMap.ecrireFichierObj();
		break;
	case 'm':
	case 'M':
		maMap.setIsDilated(true);
		maMap.setDilatation(maMap.getDilatation() + 0.1);	
		monVBO.FeedCol(maMap.getCol());
		monVBO.FeedPos(maMap.getPos());
		monVBO.ActualizeColBuffer();
		monVBO.ActualizePosBuffer();
		break;
	case 'l':
	case 'L':
		maMap.setIsDilated(true);
		maMap.setDilatation(maMap.getDilatation() - 0.1);	
		monVBO.FeedCol(maMap.getCol());
		monVBO.FeedPos(maMap.getPos());
		monVBO.ActualizeColBuffer();
		monVBO.ActualizePosBuffer();
		break;
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
	// On ne fait quelque chose que sur le bouton gauche de la souris
	if (bouton == GLUT_LEFT_BUTTON) {
		// si on relache le bouton on met a jour les angles theta et phi
		// et on dit que l'on a pas clique
		if (etat == GLUT_UP) 
		{
			camera.setBouttonUp();
		}
		else  
		{	// state = GLUT_DOWN
			// si l'on a clique sur le bouton gauche
			// on garde les positions de la souris au moment du clic gauche
			camera.setBouttonDown(x,y);
		}
	}
}

// Fonction de gestion du deplacement de la souris
void deplacementSouris(int x, int y) {

	bool affiche = false;

	//on actualise les vecteurs de la caméra
	affiche = camera.ActualiserCamera(x, y);

	//on n'affiche que si on est en train de cliquer
	if (affiche){ glutPostRedisplay();}
}

// Callback de redimensionnement de la fenêtre
GLvoid redimensionner(int _windowW, int _windowH){//float _focale, float _near, float _far) {
	
	// Garde les valeurs
	int windowW = _windowW;
	int windowH = _windowH;
	// eviter une division par 0
	if(windowH==0)
		windowH = 1;

	float ratio = static_cast<float>(windowW) / static_cast<float>(windowH);
	cout << "Ratio : " << ratio << endl;

	// Projection
	glMatrixMode(GL_PROJECTION);

	// Resetting matrix
	glLoadIdentity();

	// Viewport
	glViewport(0, 0, windowW, windowH);

	// Mise en place de la perspective
	gluPerspective(camera.getFocale(), 1.0, camera.getNear(), camera.getFar());

	// Retourne a la pile modelview
	glMatrixMode(GL_MODELVIEW);
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
		camera.resetDeltaStrafe();
		break;

		// On arrete d'avance/reculer
	case 'z':
	case 's':
	case 'Z':
	case 'S':
		camera.resetDeltaMove();
		break;
	}
}

// Fonction de gestion du clavier (touche speciale relachee)
void releaseSpecialKey(int key, int x, int y) {

	switch (key) {
		// On remet le delta deplacement a 0
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		camera.resetDeltaMove();
		break;

		// On remet le delta strafe a 0
	case GLUT_KEY_RIGHT:
	case GLUT_KEY_LEFT:
		camera.resetDeltaStrafe();
		break;
	}
}