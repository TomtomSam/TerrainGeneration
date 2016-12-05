// Include des libraires
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include "SOIL.h"

// Include des headers de classes
#include "heightMap.h"
#include "vector3D.h" 
#include "FreeFlyCamera.h"
#include "VBO.h"
#include "Texture.h"
#include "Chrono.h"

// Initialisation de la carte
heightMap maMap(11);

// Creation des textures
//Texture water;
//Texture grass;

// Creation du chrono
Chrono chrono;

// Initialisation du Vertex Buffer Object
VBO monVBO;

// Initialisation du nombre de points par ligne/colonne
int taille = static_cast<int>(pow(2, maMap.getLength()));

//Initialisation camera: (MoveSensitivity,CampPos,TargetPos)
FreeFlyCamera cam(static_cast<float>(taille)/100, 0.5*taille, taille, -0.5*taille, 0, -0.5*taille, 0.75*taille);

// Initliasitation des proprietes de la fenêtre
int windowW = 1000;
int windowH = 550;
float focale = 70.0f;
float Near = 0.1f;
float Far = (float)taille*2;
int FPS = 0;

// Déclarations des fonctions de rappel (callbacks)
GLvoid affichage();
GLvoid clavier(unsigned char touche, int x, int y);
GLvoid souris(int bouton, int etat, int x, int y);
GLvoid deplacementSouris(int x, int y);
GLvoid redimensionner(int w, int h);
GLvoid dilaterMap(int bouton, int dir, int x, int y);

void compteurFPS();
void dessinOcean();
void dessinCacheMisere();

int p = 0;

// Definition de la fonction d'affichage
GLvoid affichage(){
	// Declenchement du chrono
	chrono.Tic();

	// On gere le deplacement de la camera
	cam.cameraMovement();

	// Effacement du frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//***************
	//COMPTEUR DE FPS
	//***************
	compteurFPS();//Affiche les FPS 

	//ON REPASSE EN MODE PERSPECTIVE POUR LA MAP
	// Projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Viewport
	glViewport(0, 0, windowW, windowH);

	// Mise en place de la perspective
	Far = (float)taille * 2 * maMap.getDilatation();
	gluPerspective(focale, 1.0, Near, Far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Definition de la position de la camera et ou elle regarde
	gluLookAt(	cam.getcamPos().getVx()		, cam.getcamPos().getVy()	, cam.getcamPos().getVz()	, 
				cam.gettargetPos().getVx()	, cam.gettargetPos().getVy(), cam.gettargetPos().getVz(), 
				cam.getupWorld().getVx()	, cam.getupWorld().getVy()	, cam.getupWorld().getVz()	);

	
	//*****************
	// DESSIN DE LA MAP
	//*****************
	/*glBindTexture(GL_TEXTURE_2D, water.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);*/
	monVBO.DrawBuffer();
	//****************
	// LE CACHE MISERE
	//****************
	dessinCacheMisere(); //Cache le dessous de la map
	
	//*************
	// DESSIN OCEAN
	//*************
	dessinOcean();


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


	FPS = static_cast<int>(1000 / static_cast<float>(chrono.getEllapsed_time()));
}


void compteurFPS()
{
	//Il faut mettre le text en projection orthographique
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, windowW, 0.0, windowH);
	//Affichage du texte
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Couleur du texte
	if (FPS>55){ glColor3f(0.0f, 1.0f, 0.0f); }
	else if (FPS<56 && FPS>29){ glColor3f(1.0f, 0.5f, 0.0f); }
	else if (FPS<30){ glColor3f(1.0f, 0.0f, 0.0f); }

	glRasterPos2i(10, windowH - 30);
	string s = to_string(FPS) + " FPS";

	void * font = GLUT_BITMAP_TIMES_ROMAN_24;
	//void * font = GLUT_BITMAP_HELVETICA_18; 
	//void * font = GLUT_BITMAP_9_BY_15;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
}

void dessinOcean()
{
	/*glBindTexture(GL_TEXTURE_2D, water.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);*/
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.75);
	glTexCoord2f(0, 0);
	glVertex3f(0, maMap.getPosOcean(), 0);
	glTexCoord2f(1, 0);
	glVertex3f(taille*maMap.getDilatation(), maMap.getPosOcean(), 0);
	glTexCoord2f(1, 1);
	glVertex3f(taille*maMap.getDilatation(), maMap.getPosOcean(), taille*maMap.getDilatation());
	glTexCoord2f(0, 1);
	glVertex3f(0, maMap.getPosOcean(), taille*maMap.getDilatation());
	glEnd();
}

void dessinCacheMisere()
{
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0, 0, 0);
	//on commence en (0,0)
	//à droite
	for (int i = 0; i <= taille; i++)
	{
		glVertex3f(0, maMap.getHeightMap(0, i)->getHeight()*(1 + (maMap.getDilatation() - 1) / 3), i*maMap.getDilatation());
		glVertex3f(0, maMap.getPosOcean(), i*maMap.getDilatation());
	}
	//en bas
	for (int i = 0; i <= taille; i++)
	{
		glVertex3f(i*maMap.getDilatation(), maMap.getHeightMap(i, taille)->getHeight()*(1 + (maMap.getDilatation() - 1) / 3), taille*maMap.getDilatation());
		glVertex3f(i*maMap.getDilatation(), maMap.getPosOcean(), taille*maMap.getDilatation());
	}
	//à gauche
	for (int i = taille; i >= 0; i--)
	{
		glVertex3f(taille*maMap.getDilatation(), maMap.getHeightMap(taille, i)->getHeight()*(1 + (maMap.getDilatation() - 1) / 3), i*maMap.getDilatation());
		glVertex3f(taille*maMap.getDilatation(), maMap.getPosOcean(), i*maMap.getDilatation());
	}
	//en haut
	for (int i = taille; i >= 0; i--)
	{
		glVertex3f(i*maMap.getDilatation(), maMap.getHeightMap(i, 0)->getHeight()*(1 + (maMap.getDilatation() - 1) / 3), 0);
		glVertex3f(i*maMap.getDilatation(), maMap.getPosOcean(), 0);
	}
	glEnd();
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
	case '+': //changer le seuil de l'océan
		maMap.setPosOcean(maMap.getPosOcean()+1);
		monVBO.FeedCol( maMap.getCol());
		monVBO.ActualizeColBuffer();
		break;
	case '-':
		maMap.setPosOcean(maMap.getPosOcean() - 1);
		monVBO.FeedCol(maMap.getCol());
		monVBO.ActualizeColBuffer();
		break;
	case 'w':
		maMap.ecrireFichierObj();
		break;
	case 27:
		monVBO.DestroyVBO();
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
	Far = (float)taille *2* maMap.getDilatation();
	gluPerspective(focale, 1.0, Near, Far);

	// Retourne a la pile modelview
	glMatrixMode(GL_MODELVIEW);
}

GLvoid dilaterMap(int bouton, int dir, int x, int y)
{
	//La map est dilatée donc il va falloir actualiser la pos de l'océan
	maMap.setIsDilated(true);
	if (dir > 0)
	{
		maMap.setDilatation(maMap.getDilatation() + 0.1);	
	}
	else if (maMap.getDilatation()>1.0f)
	{
		maMap.setDilatation(maMap.getDilatation() - 0.1);
	}

	//On actualise le VBO
	monVBO.FeedCol(maMap.getCol());
	monVBO.FeedPos(maMap.getPos());
	monVBO.ActualizeColBuffer();
	monVBO.ActualizePosBuffer();
	
	// Reaffichage de la scene
	glutPostRedisplay();
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
	//Initialisation de GLU
	glewInit();
	// Définition de la couleur d'effacement du framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Activation Z-buffer
	glEnable(GL_DEPTH_TEST);


	//// At initialization
	//glEnable(GL_TEXTURE_2D);
	////Load/Bind Textures
	//water.loadTexture("WATER.jpg");
	//grass.loadTexture("GRASS.jpg");
	//// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	//Initialisation Map
	maMap.initialisationAuto();
	//On génère la heightMap
	maMap.generateMatrix();
	//On rempli les vecteurs de Data à envoyer au GPU
	maMap.FillDataBuffersPosColors();

	//On rempli le VBO
	monVBO.FeedCol(maMap.getCol());
	monVBO.FeedPos(maMap.getPos());
	monVBO.FeedTex(maMap.getTex());
	monVBO.BuildBuffer();

	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutKeyboardUpFunc(clavierUp);
	glutSpecialUpFunc(releaseSpecialKey);

	// Nouveaux callbacks
	glutMouseFunc(souris);
	glutMotionFunc(deplacementSouris);
	glutReshapeFunc(redimensionner);
	glutMouseWheelFunc(dilaterMap);

	// Lancement de la boucle infinie GLUT
	glutMainLoop();

	return 0;
}
