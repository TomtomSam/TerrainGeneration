/******************************************************************************/
/************************ FRACTAL LANDSCAPES GENERATION ***********************/
/********************* by Bastien TOUBHANS & Thomas SAMUEL ********************/
/**************************** created on 06/10/2016 ***************************/
/**************************** updated on 08/12/2016 ***************************/
/******************************************************************************/

// Choix du namespace
using namespace std;

// Include des libraires
#include <Windows.h>

// Include des headers de classes
#include "Texture.h"
//#include "Program.h"
#include "CallbackHandler.h"
#include "FreeFlyCamera.h"

// Initialisation de la carte
heightMap maMap(7);

//// Creation des textures
//Texture water;
//Texture grass;
//Texture ice;
//Texture sand;

// Creation du chrono
Chrono chrono;

// Initialisation du Vertex Buffer Object
VBO monVBO;

// Initialisation du nombre de points par ligne/colonne
int taille = static_cast<int>(pow(2, maMap.getLength()));

//Initialisation camera: (MoveSensitivity,CampPos,TargetPos)
FreeFlyCamera camera(static_cast<float>(taille)/100, 0.5*taille, taille, -0.5*taille, 0, -0.5*taille, 0.75*taille);

// Initliasitation des proprietes de la fenêtre
int windowW = 1000;
int windowH = 550;
float focale = 70.0f;
float Near = 0.5f;
float Far = static_cast<float>(taille*2);
int FPS = 0;

// Déclarations des fonctions de rappel (callbacks)
GLvoid affichage();
	//GLvoid clavier(unsigned char touche, int x, int y);
	//GLvoid souris(int bouton, int etat, int x, int y);
	//GLvoid deplacementSouris(int x, int y);
void dessinOcean();
void dessinCacheMisere();

// Definition de la fonction d'affichage
GLvoid affichage(){
	// Declenchement du chrono
	chrono.Tic();

	// On gere le deplacement de la camera
	camera.cameraMovement();

	// Effacement du frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//***************
	//COMPTEUR DE FPS
	//***************
	maMap.compteurFPS(windowW, windowH, FPS);//Affiche les FPS 

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
	gluLookAt(	camera.getcamPos().getVx()		, camera.getcamPos().getVy()	, camera.getcamPos().getVz()	, 
		camera.gettargetPos().getVx()	, camera.gettargetPos().getVy(), camera.gettargetPos().getVz(), 
		camera.getupWorld().getVx()	, camera.getupWorld().getVy()	, camera.getupWorld().getVz()	);

	//*****************
	// DESSIN DE LA MAP
	//*****************
	/*glBindTexture(GL_TEXTURE_2D, water.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);*/
	monVBO.DrawBuffer();
	//****************
	// LE CACHE MISERE
	//****************
	maMap.dessinCacheMisere(); //Cache le dessous de la map

	//*************
	// DESSIN OCEAN
	//*************
	maMap.dessinOcean();

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

	// At initialization
	glEnable(GL_TEXTURE_2D);

	//Load/Bind Textures
	/*water.loadTexture("WATER.jpg");
	sand.loadTexture("SAND.jpg");
	ice.loadTexture("ICE.jpg");
	grass.loadTexture("GRASS.jpg");*/

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

	//Reglages camera
	camera.setFar(static_cast<float>(taille*2));

	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutKeyboardUpFunc(clavierUp);
	glutSpecialUpFunc(releaseSpecialKey);
	glutMouseFunc(souris);
	glutMotionFunc(deplacementSouris);
	glutReshapeFunc(redimensionner);

	//Min et Max des altitudes de la carte
	float maxMin[2];
	maMap.giveMaxes(maxMin);

	//// Creation des shaders et du program associe
	//Shader VS,FS; 
	//Program prog;
	//VS.loadShader("vertexShader.vert", GL_VERTEX_SHADER);
	//FS.loadShader("fragmentShader.frag", GL_FRAGMENT_SHADER);
	//prog.createProgram();
	//prog.addShaderToProgram(&VS);
	//prog.addShaderToProgram(&FS);
	//prog.linkProgram(&VS, &FS);
	//prog.useProgram();

	////Assignation des variables des shaders
	//prog.setUniformf("maxHeight", maxMin[0]);
	//prog.setUniformf("minHeight", maxMin[1]);
	//prog.setUniformf("beachTransition", (maxMin[0]-maxMin[1])*0.6);
	//prog.setUniformf("snowTransition", (maxMin[0]-maxMin[1])*0.9);
	//prog.setUniformf("oceanTransition", (maxMin[0]-maxMin[1])*0.3);

	//prog.setUniformi("waterID", water.getTexture());
	//prog.setUniformi("grassID", grass.getTexture());
	//prog.setUniformi("iceID", ice.getTexture());
	//prog.setUniformi("sandID", sand.getTexture());
	//GLint texture, texture1, texture2, texture3;
	//glActiveTexture(GL_TEXTURE0 + water.getTexture());
	//glBindTexture(GL_TEXTURE_2D, water.getTexture());
	//texture = glGetUniformLocation(prog.getProgramID(), "tex_water");
	//glUniform1i(texture, 0);
	//glActiveTexture(GL_TEXTURE0 + grass.getTexture());
	//glBindTexture(GL_TEXTURE_2D, grass.getTexture());
	//texture1 = glGetUniformLocation(prog.getProgramID(), "tex_grass");
	//glUniform1i(texture1, 1);
	//glActiveTexture(GL_TEXTURE0 + ice.getTexture());
	//glBindTexture(GL_TEXTURE_2D, ice.getTexture()); 
	//texture2 = glGetUniformLocation(prog.getProgramID(), "tex_ice");
	//glUniform1i(texture2, 2);
	//glActiveTexture(GL_TEXTURE0 + sand.getTexture());
	//glBindTexture(GL_TEXTURE_2D, sand.getTexture()); 
	//texture3 = glGetUniformLocation(prog.getProgramID(), "tex_sand");
	//glUniform1i(texture3, 3);

	// Lancement de la boucle infinie GLUT
	glutMainLoop();

	return 0;
}
