// Include des libraires
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include "SOIL.h"

// Include des headers de classes
#include "heightMap.h"
#include "FreeFlyCamera.h"
#include "Texture.h"
#include "Chrono.h"
// Classe VBO

// Initialisation de la carte
heightMap maMap(10);

// Creation des textures
Texture water;
Texture grass;
Texture ice;
Texture sand;

// Creation du chrono
Chrono chrono;

// Initialisation du nombre de points par ligne/colonne
int taille = pow(2, maMap.getLength());

//Initialisation camera: (MoveSensitivity,CampPos,TargetPos)
FreeFlyCamera cam(static_cast<float>(taille)/100, 0.5*taille, taille, -0.5*taille, 0, -0.5*taille, 0.75*taille);

// Creation des variables utilisees par le chrono
int last_time = glutGet(GLUT_ELAPSED_TIME);
int current_time, ellapsed_time;

// Vertex Data 
vector<float> pos;
vector<float> colors;
float color[3];

// Initialisation du Vertex Buffer Object
GLuint bufferMap;
#define BUFFER_OFFSET(a) ((char*)NULL + (a))

// Creation des variables utilisees pour la gestion des seuils de textures
Seuil neige;
Seuil plage;
Seuil ocean;

// Initliasitation des proprietes de la fenêtre
int windowW = 1000;
int windowH = 550;
float focale = 70.0f;
float Near = 0.1f;
float Far = static_cast<float>(taille*2);

// Déclarations des fonctions de rappel (callbacks)
GLvoid affichage();
GLvoid clavier(unsigned char touche, int x, int y);
GLvoid souris(int bouton, int etat, int x, int y);
GLvoid deplacementSouris(int x, int y);
GLvoid redimensionner(int w, int h);

// Remplissage du VBO
void FillDataBuffers()
{
	chrono.Tic();

	pos.clear();
	colors.clear();

	int j = 0;

	// Remplissage strip par strip
	for (int i = 0; i < taille; i = i + 2)
	{

		// Remplissage strip vers la droite
		for (j = 0; j <= taille; j++)
		{
			// Les Positions 
			pos.push_back(i);
			pos.push_back(maMap.getHeightMap(i, j));
			pos.push_back(j);
			pos.push_back(i + 1);
			pos.push_back(maMap.getHeightMap(i + 1, j));
			pos.push_back(j);

			// Les couleurs
			maMap.vertexColor(maMap.getHeightMap(i, j), neige, plage, ocean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
			maMap.vertexColor(maMap.getHeightMap(i + 1, j), neige, plage, ocean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
		}
		// Le dernier point de chaque strip est rentré deux fois dans le vecteur pour faire le virage
		// Remplissage strip vers la gauche
		for (j = taille; j >= 0; j--)
		{
			// Les Positions
			pos.push_back(i + 1);
			pos.push_back(maMap.getHeightMap(i + 1, j));
			pos.push_back(j);
			pos.push_back(i + 2);
			pos.push_back(maMap.getHeightMap(i + 2, j));
			pos.push_back(j);

			//Les couleurs
			maMap.vertexColor(maMap.getHeightMap(i + 1, j), neige, plage, ocean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
			maMap.vertexColor(maMap.getHeightMap(i + 2, j), neige, plage, ocean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
		}
	}

	chrono.Toc();
	cout << "Remplissage des donnees effectue en: " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s." << endl;

}

// Dessin du VBO
void BuildAndDrawBuffer()
{
#define P_SIZE 3
#define C_SIZE 3

	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferMap);

	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferMap);

	// On alloue l'espace necessaire en memoire
	// Il y a 2^n strips à tracer et chaque strip contient 2*(2^n+1) sommets
	glBufferData(GL_ARRAY_BUFFER,                   // Cible 
		((2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0]) +  // Taille des positions
		((2 * (taille + 1)*taille)*C_SIZE*sizeof colors[0]),// Taille des couleurs
		NULL,                           
		GL_STREAM_DRAW);                // Mode de dessin

	// Specification des donnees
	glBufferSubData(GL_ARRAY_BUFFER,
		0,                            // emplacement des donnees dans le VBO
		((2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0]), // Taille des donnees
		&pos[0]);                         // Adresse des donnees

	glBufferSubData(GL_ARRAY_BUFFER,
		(2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0],   // Emplacement
		((2 * (taille + 1)*taille)*C_SIZE*sizeof pos[0]), // Taille
		&colors[0]);                        // Adresse

	glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET((2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0]));

	// Activation des tableaux de sommets 
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Dessin des strips
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (taille + 1)*taille);

	// Desactivation des tableaux de sommets
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Liberation de la place en memmoire
	glDeleteBuffers(1, &bufferMap);
}


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
	gluLookAt(cam.getcamPos().getVx(), cam.getcamPos().getVy(), cam.getcamPos().getVz(), cam.gettargetPos().getVx(), cam.gettargetPos().getVy(), cam.gettargetPos().getVz(), cam.getupWorld().getVx(), cam.getupWorld().getVy(), cam.getupWorld().getVz());

	// Dessin de la map
	BuildAndDrawBuffer();

	// Dessin de l'ocean
	glBegin(GL_QUADS);
		glBindTexture(GL_TEXTURE_2D, water.getTexture());
		glColor3f(0, 0, 0.75);
		glTexCoord2f(0, 0);
		glVertex3f(0, ocean.getHauteur(), 0);
		glTexCoord2f(1, 0);
		glVertex3f(taille, ocean.getHauteur(), 0);
		glTexCoord2f(1, 1);
		glVertex3f(taille, ocean.getHauteur(), taille);
		glTexCoord2f(1, 0);
		glVertex3f(0, ocean.getHauteur(), taille);
		glBindTexture(GL_TEXTURE_2D, 0);
	glEnd();

	// Mettre ça dans un VBO
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0, 0, 0);
	//on commrnce en (0,0)
	//à droite
	for (int i = 0; i <= taille; i++)
	{
		glVertex3f(0, maMap.getHeightMap(0, i), i);
		glVertex3f(0, ocean.getHauteur(), i);
	}
	//en bas
	for (int i = 0; i <= taille; i++)
	{
		glVertex3f(i, maMap.getHeightMap(i, taille), taille);
		glVertex3f(i, ocean.getHauteur(), taille);
	}
	//à gauche
	for (int i = taille; i >= 0; i--)
	{
		glVertex3f(taille, maMap.getHeightMap(taille, i), i);
		glVertex3f(taille, ocean.getHauteur(), i);
	}
	//en haut
	for (int i = taille; i >= 0; i--)
	{
		glVertex3f(i, maMap.getHeightMap(i, 0), 0);
		glVertex3f(i, ocean.getHauteur(), 0);
	}
	glEnd();

	// Affichage ecran
	glFlush();
	glutSwapBuffers();

	// Compteur FPS
	chrono.Toc();

	if (chrono.getEllapsed_time() < 17)
	{ 
		Sleep(17 - chrono.getEllapsed_time());
	}

	chrono.Toc();
	cout << "FPS: " << chrono.getEllapsed_time() << endl;

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
		ocean++;
		break;
	case '-':
		ocean--;
		break;
	case 'f':          //changer le seuil de neige
	case 'F':
		neige++;
		//?????????????????????????????????????????? Ajouter un recalcul des textures de la map
		FillDataBuffers();
		break;
	case 'v':
	case 'V':
		neige--;
		FillDataBuffers();
		break;
	case 'g':			//changer le seuil de la plage
	case 'G':
		plage++;
		FillDataBuffers();
		break;
	case 'b':
	case 'B':
		plage--;
		FillDataBuffers();
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
			int h = 2;
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

	chrono.Tic();
	//Initialisation Map
	maMap.initialisationAuto();
	//On génère la heightMap
	maMap.generateMatrix();
	chrono.Toc();
	cout << "Generation effectuee en " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s." << endl;

	//Min et Max des altitudes de la carte
	float maxMin[2];
	maMap.giveMaxes(maxMin);
	cout << "Max: " << maxMin[0] << " Min: " << maxMin[1] << endl;
	cout << maxMin[0]-maxMin[1] <<endl;

	//Min et Max des altitudes de la carte
	neige = maMap.seuilDefinition(neige, 0.8);
	plage = maMap.seuilDefinition(plage, 0.32);
	ocean = maMap.seuilDefinition(ocean, 0.3);

	cout << "neige: " << neige.getHauteur() << " plage: " << plage.getHauteur() << " ocean: " << ocean.getHauteur() << endl;

	//On rempli les données à envoyer au GPU
	FillDataBuffers();

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

	// At initialization
	glEnable(GL_TEXTURE_2D);
	water.loadTexture("WATER.jpg");
	grass.loadTexture("GRASS.jpg");
	ice.loadTexture("ICE.jpg");
	sand.loadTexture("SAND.jpg");
	glBindTexture(GL_TEXTURE_2D, water.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, grass.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, ice.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, sand.getTexture());
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

	// Blending
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialisation de GLU
	glewInit();

	// Définition de la couleur d'effacement du framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);

	// pour que l'on puisse rester appuye sur les touches
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
