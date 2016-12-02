/******************************************************************************/
/************************ FRACTAL LANDSCAPES GENERATION ***********************/
/********************* by Bastien TOUBHANS & Thomas SAMUEL ********************/
/**************************** created on 06/10/2016 ***************************/
/**************************** updated on 30/11/2016 ***************************/
/******************************************************************************/

// Choix du namespace
using namespace std;


// Include des libraires
#include <Windows.h>

// Include des headers de classes
#include "heightMap.h"
#include "FreeFlyCamera.h"
#include "VBO.h"
#include "Texture.h"
#include "Program.h"
#include <iostream>

// Initialisation de la carte
heightMap maMap(7);

// Creation des textures
Texture water;
Texture grass;

// Creation du chrono
Chrono chrono;

// Initialisation du Vertex Buffer Object
VBO monVBO;

// Initialisation du nombre de points par ligne/colonne
int taille = pow(2, maMap.getLength());

//Initialisation camera: (MoveSensitivity,CampPos,TargetPos)
FreeFlyCamera cam(static_cast<float>(taille)/100, 0.5*taille, taille, -0.5*taille, 0, -0.5*taille, 0.75*taille);

// Initliasitation des proprietes de la fenêtre
int windowW = 1000;
int windowH = 550;
float focale = 70.0f;
float Near = 0.1f;
float Far = (float)taille*2;

// Déclarations des fonctions de rappel (callbacks)
GLvoid affichage();
GLvoid clavier(unsigned char touche, int x, int y);
GLvoid souris(int bouton, int etat, int x, int y);
GLvoid deplacementSouris(int x, int y);
GLvoid redimensionner(int w, int h);



// Definition de la fonction d'affichage
GLvoid affichage(){
	// Declenchement du chrono
	chrono.Tic();

	// On gere le deplacement de la camera
	cam.cameraMovement();

	// Effacement du frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Definition de la position de la camera et ou elle regarde
	gluLookAt(	cam.getcamPos().getVx()		, cam.getcamPos().getVy()	, cam.getcamPos().getVz()	, 
		cam.gettargetPos().getVx()	, cam.gettargetPos().getVy(), cam.gettargetPos().getVz(), 
		cam.getupWorld().getVx()	, cam.getupWorld().getVy()	, cam.getupWorld().getVz()	);

	// DESSIN DE LA MAP
	glBindTexture(GL_TEXTURE_2D, grass.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	monVBO.BuildAndDrawBuffer();


	//Le cache Misère sur les cotés de la map
	// TODO:Mettre ça dans un VBO
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0, 0, 0);
	//on commence en (0,0)
	//à droite
	for (int i = 0; i <= taille; i++)
	{
		glVertex3f(0, maMap.getHeightMap(0, i)->getHeight(), i);
		glVertex3f(0, maMap.getPosOcean(), i);
	}
	//en bas
	for (int i = 0; i <= taille; i++)
	{
		glVertex3f(i, maMap.getHeightMap(i, taille)->getHeight(), taille);
		glVertex3f(i, maMap.getPosOcean(), taille);
	}
	//à gauche
	for (int i = taille; i >= 0; i--)
	{
		glVertex3f(taille, maMap.getHeightMap(taille, i)->getHeight(), i);
		glVertex3f(taille, maMap.getPosOcean(), i);
	}
	//en haut
	for (int i = taille; i >= 0; i--)
	{
		glVertex3f(i, maMap.getHeightMap(i, 0)->getHeight(), 0);
		glVertex3f(i, maMap.getPosOcean(), 0);
	}
	glEnd();


	// Dessin de l'ocean
	glBindTexture(GL_TEXTURE_2D, water.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.75);
	glTexCoord2f(0, 0);
	glVertex3f(0, maMap.getPosOcean(), 0);
	glTexCoord2f(1, 0);
	glVertex3f(taille, maMap.getPosOcean(), 0);
	glTexCoord2f(1, 1);
	glVertex3f(taille, maMap.getPosOcean(), taille);
	glTexCoord2f(0, 1);
	glVertex3f(0, maMap.getPosOcean(), taille);
	glEnd();

	// Affichage ecran
	glFlush();
	glutSwapBuffers();

	// Compteur FPS
	chrono.Toc();

	if (chrono.getEllapsed_time() < 17)
	{ 
		Sleep(17 - chrono.getEllapsed_time());
		chrono.Toc();
	}

	cout << "FPS: " << static_cast<int>(1000 / static_cast<float>(chrono.getEllapsed_time())) << endl;

}

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
		break; */
	case '+': //changer le seuil de l'océan
		maMap.setPosOcean(maMap.getPosOcean()+1);
		monVBO.FeedData(maMap.getPos(), maMap.getCol(), maMap.getTex());
		break;
	case '-':
		maMap.setPosOcean(maMap.getPosOcean()-1);
		monVBO.FeedData(maMap.getPos(), maMap.getCol(), maMap.getTex());
		break;
	case 'w':
		maMap.ecrireFichierObj();
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


// Callback de redimensionnement de la fenêtre
GLvoid redimensionner(int w, int h) {
	// Garde les valeurs
	windowW = w;
	windowH = h;
	// eviter une division par 0
	if(windowH==0)
		windowH = 1;

	float ratio = (float)windowW / (float)windowH;
	cout << "Ratio : " << ratio << endl;


	// Projection
	glMatrixMode(GL_PROJECTION);

	// Resetting matrix
	glLoadIdentity();

	// Viewport
	glViewport(0, 0, windowW, windowH);

	// Mise en place de la perspective
	gluPerspective(focale, 1.0, Near, Far);

	// Retourne a la pile modelview
	glMatrixMode(GL_MODELVIEW);
}

int main (int argc, char *argv[])
{
	srand(time(NULL));

	// Initialisation de GLUT
	glutInit(&argc, argv);
	// Choix du mode d'affichage (ici RVB)
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	// Position initiale de la fenetre GLUT
	glutInitWindowPosition(200, 70);
	// Taille initiale de la fenetre GLUT
	glutInitWindowSize(windowW, windowH);
	// Creation de la fenetre GLUT
	glutCreateWindow("Map Generation");
	//Initialisation de GLEW
	glewInit();
	// Définition de la couleur d'effacement du framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Activation Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Creation des shaders
	Shader VS,FS; 
	Program prog;
	VS.loadShader("vertexShader.vert", GL_VERTEX_SHADER);
	FS.loadShader("fragmentShader.frag", GL_FRAGMENT_SHADER);
	prog.createProgram();
	prog.addShaderToProgram(&VS);
	prog.addShaderToProgram(&FS);
	prog.linkProgram(&VS, &FS);
	prog.useProgram();

	// At initialization
	glEnable(GL_TEXTURE_2D);
	//Load/Bind Textures
	water.loadTexture("WATER.jpg");
	grass.loadTexture("GRASS.jpg");
	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_COLOR, GL_DST_COLOR, GL_ZERO, GL_ONE);

	//Initialisation Map
	maMap.initialisationAuto();
	//On génère la heightMap
	maMap.generateMatrix();

	//Min et Max des altitudes de la carte
	float maxMin[2];
	maMap.giveMaxes(maxMin);
	cout << "Max: " << maxMin[0] << " Min: " << maxMin[1] << endl;
	cout << maxMin[0] - maxMin[1] << endl;

	//On rempli les données à envoyer au GPU
	maMap.FillDataBuffersPosColors();
	monVBO.FeedData(maMap.getPos(), maMap.getCol(), maMap.getTex());

	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutKeyboardUpFunc(clavierUp);
	glutSpecialUpFunc(releaseSpecialKey);

	// Nouveaux callbacks
	glutMouseFunc(souris);
	glutMotionFunc(deplacementSouris);
	glutReshapeFunc(redimensionner);

	// Lancement de la boucle infinie GLUT
	glutMainLoop();


	return 0;
}
