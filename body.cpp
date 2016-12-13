/******************************************************************************/
/************************ FRACTAL LANDSCAPES GENERATION ***********************/
/********************* by Bastien TOUBHANS & Thomas SAMUEL ********************/
/**************************** created on 06/10/2016 ***************************/
/**************************** updated on 09/12/2016 ***************************/
/******************************************************************************/

// Choix du namespace
using namespace std;

// Inclusion des libraires
#include <Windows.h>

// Inclusion des headers de classes
#include "Texture.h"
//#include "Program.h"
#include "CallbackHandler.h"


// Initialisation de la carte
heightMap maMap(5);

//// Creation des textures
//Texture water;
//Texture grass;
//Texture ice;
//Texture sand;



// Creation du chrono
Chrono chrono;

// Initialisation du Vertex Buffer Object
VBO monVBO;

// Creation et initialisation  de la camera
FreeFlyCamera camera(static_cast<float>(maMap.getTaille()) / 100, 0.5*maMap.getTaille(), maMap.getTaille(), -0.5*maMap.getTaille(), 0, -0.5*maMap.getTaille(), 0.75*maMap.getTaille());

// Initialisation des proprietes de la fenêtre
int windowW = 1000;
int windowH = 550;
int FPS = 0;

InterfaceUtilisateur IU(&maMap, &monVBO,&camera);

// Definition de la fonction d'affichage
GLvoid affichage()
{	
	// Declenchement du chrono
	chrono.Tic();

	// Gestion du deplacement de la camera
	camera.cameraMovement();

	// Effacement du frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Choix de la projection orthographique pour le texte
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, windowW, 0.0, windowH);
	// Affichage du compteur de FPS
	maMap.compteurFPS(windowW, windowH, FPS);

	//DESSIN DE LINTERFACE GRAPHIQUE
	IU.draw(windowW,windowH);

	// Passage en mode perspective pour afficher la map
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Gestion du viewport
	glViewport(0, 0, windowW, windowH);

	// Mise en place de la perspective
	camera.setFar(maMap.getTaille() * 2 * maMap.getDilatation());
	gluPerspective(camera.getFocale(), 1.0, camera.getNear(), camera.getFar());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Definition de la position de la camera et ou elle regarde
	gluLookAt(	camera.getcamPos().getVx(), camera.getcamPos().getVy(), camera.getcamPos().getVz(), 
		camera.gettargetPos().getVx(), camera.gettargetPos().getVy(), camera.gettargetPos().getVz(), 
		camera.getupWorld().getVx(), camera.getupWorld().getVy(), camera.getupWorld().getVz()	);

	// Dessin de la map
	monVBO.DrawBuffer();

	// Dessin de l'ocean
	maMap.dessinOcean();

	// Affichage a l'ecran
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

	// Initialisation de GLEW
	glewInit();

	// Définition de la couleur d'effacement du framebuffer
	//NOIR
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//BLEU MINUIT
	glClearColor(0.0f, 0.2f, 0.4f, 0.0f);
	

	// Activation du Z-buffer
	glEnable(GL_DEPTH_TEST);
	//Affichage du dessus de la map uniquement
	glPolygonMode(GL_BACK, GL_FILL);

	// Activation des textures
	glEnable(GL_TEXTURE_2D);

	//// Chargement des textures
	//water.loadTexture("WATER.jpg");
	//sand.loadTexture("SAND.jpg");
	//ice.loadTexture("ICE.jpg");
	//grass.loadTexture("GRASS.jpg");

	// Initialisation de la map
	maMap.initialisationAuto();

	// Generation de la map
	maMap.generateMatrix();

	// Remplissage des vecteurs de data a envoyer au GPU
	maMap.FillDataBuffersPosColors();

	// Remplissage du VBO
	monVBO.FeedCol(maMap.getCol());
	monVBO.FeedPos(maMap.getPos());
	monVBO.FeedTex(maMap.getTex());
	monVBO.BuildBuffer();

	// Reglages de la camera
	camera.setFar(static_cast<float>(maMap.getTaille() * 2));

	// Definition des fonctions de callback
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutKeyboardUpFunc(clavierUp);
	glutMouseFunc(souris);
	glutMotionFunc(deplacementSouris);
	glutReshapeFunc(redimensionner);
	glutPassiveMotionFunc(deplacementSourisPassif);

	// Calcul des mins et max des altitudes de la carte
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

	//// Assignation des variables des shaders
	//prog.setUniformf("maxHeight", maxMin[0]);
	//prog.setUniformf("minHeight", maxMin[1]);
	//prog.setUniformf("grassInf", maMap.getPosOcean()+20);
	//prog.setUniformf("grassSup", maMap.getPosOcean()+410);
	//prog.setUniformf("beachInf", maMap.getPosOcean()+1);
	//prog.setUniformf("beachSup", maMap.getPosOcean()+30);
	//prog.setUniformf("snowInf", maMap.getPosOcean()+400);
	//prog.setUniformf("waterSup", maMap.getPosOcean()+1);

	//prog.setUniformi("waterID", water.getTexture());
	//prog.setUniformi("grassID", grass.getTexture());
	//prog.setUniformi("iceID", ice.getTexture());
	//prog.setUniformi("sandID", sand.getTexture());
	//GLint texture, texture1, texture2, texture3;
	//glActiveTexture(GL_TEXTURE0 + water.getTexture());
	//glBindTexture(GL_TEXTURE_2D, water.getTexture());
	//texture = glGetUniformLocation(prog.getProgramID(), "tex_water");
	//glUniform1i(texture, 1);	
	//glActiveTexture(GL_TEXTURE0 + sand.getTexture());
	//glBindTexture(GL_TEXTURE_2D, sand.getTexture()); 
	//texture3 = glGetUniformLocation(prog.getProgramID(), "tex_sand");
	//glUniform1i(texture3, 2);	
	//glActiveTexture(GL_TEXTURE0 + ice.getTexture());
	//glBindTexture(GL_TEXTURE_2D, ice.getTexture()); 
	//texture2 = glGetUniformLocation(prog.getProgramID(), "tex_ice");
	//glUniform1i(texture2, 3);
	//glActiveTexture(GL_TEXTURE0 + grass.getTexture());
	//glBindTexture(GL_TEXTURE_2D, grass.getTexture());
	//texture1 = glGetUniformLocation(prog.getProgramID(), "tex_grass");
	//glUniform1i(texture1, 4);

	// Lancement de la boucle infinie GLUT
	glutMainLoop();

	return 0;
}
