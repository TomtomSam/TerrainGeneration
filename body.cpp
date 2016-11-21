#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

#include "heightMap.h"
#include "vector3D.h"
#include "FreeFlyCamera.h"

heightMap maMap(5);

//2^n
int taille = pow(2, maMap.getLength());

//Initialisation camera: (MoveSensitivity,CampPos,TargetPos)
FreeFlyCamera cam((float)taille / 100, 0.5*taille, taille, -0.5*taille, 0, -0.5*taille, 0.75*taille);

int last_time = glutGet(GLUT_ELAPSED_TIME);
int current_time, ellapsed_time;

//Vertex Data
vector<float> pos;
vector<float> colors;
float color[3];

//liste d'affichage
//GLuint list;
GLuint bufferMap;
#define BUFFER_OFFSET(a) ((char*)NULL + (a))

//Gestion des couleurs
float posNeige;
float posPlage;
float posOcean;

// Taille de la fenêtre
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


//demarrer le chrono
void Tic()
{
	current_time = glutGet(GLUT_ELAPSED_TIME);
	last_time = current_time;
}

//arreter le chrono
void Toc()
{
	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
}

void FillDataBuffers()
{
	Tic();

	pos.clear();
	colors.clear();

	int j = 0;
	//on rempli strip par strip
	for (int i = 0; i < taille; i = i + 2)
	{

		//première strip vers la droite
		for (j = 0; j <= taille; j++)
		{
			//Les Pos
			pos.push_back(i);
			pos.push_back(maMap.getHeightMap(i, j));
			pos.push_back(j);
			pos.push_back(i + 1);
			pos.push_back(maMap.getHeightMap(i + 1, j));
			pos.push_back(j);

			//Les couleurs
			maMap.vertexColor(maMap.getHeightMap(i, j), posNeige, posPlage, posOcean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
			maMap.vertexColor(maMap.getHeightMap(i + 1, j), posNeige, posPlage, posOcean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
		}
		//Le dernier point de chaque strip est rentré deux fois dans le vecteur pour faire le virage
		//on trace la strip vers la gauche
		for (j = taille; j >= 0; j--)
		{
			//Les Pos
			pos.push_back(i + 1);
			pos.push_back(maMap.getHeightMap(i + 1, j));
			pos.push_back(j);
			pos.push_back(i + 2);
			pos.push_back(maMap.getHeightMap(i + 2, j));
			pos.push_back(j);

			//Les couleurs
			maMap.vertexColor(maMap.getHeightMap(i + 1, j), posNeige, posPlage, posOcean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
			maMap.vertexColor(maMap.getHeightMap(i + 2, j), posNeige, posPlage, posOcean, color);
			colors.push_back(color[0]);
			colors.push_back(color[1]);
			colors.push_back(color[2]);
		}
	}

	Toc();
	cout << "Remplissage des donnees effectue en: " << (float)ellapsed_time / 1000 << "s." << endl;

}

void BuildAndDrawBuffer()
{
	#define P_SIZE 3
	#define C_SIZE 3

	/* creation d'un objet tampon et recuperation de son identifiant */
	glGenBuffers(1, &bufferMap);
	/* on bind le buffer */
	glBindBuffer(GL_ARRAY_BUFFER, bufferMap);

	/* on alloue un espace */
	//Il y a 2^n strip à tracer et chaque strip contient 2*(2^n+1) sommets
	glBufferData(GL_ARRAY_BUFFER,                   /* target */
		((2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0]) +  /* taille des positions */
		((2 * (taille + 1)*taille)*C_SIZE*sizeof colors[0]),/* taille des couleurs */
		NULL,                           /* ... */
		GL_STREAM_DRAW);                /* mode */

	/* on specifie les donnees */
	glBufferSubData(GL_ARRAY_BUFFER,
		0,                            /* emplacement des donnees dans le VBO */
		((2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0]), /* taille des donnees */
		&pos[0]);                         /* adresse des donnees */

	glBufferSubData(GL_ARRAY_BUFFER,
		(2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0],   /* emplacement */
		((2 * (taille + 1)*taille)*C_SIZE*sizeof pos[0]),/* taille */
		&colors[0]);                        /* donnees */

	glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET((2 * (taille + 1)*taille)*P_SIZE*sizeof pos[0]));

	/* activation des tableaux de sommets */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//ready

	//Dessine
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (taille + 1)*taille);

	/* desactivation des tableaux de sommets */
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDeleteBuffers(1, &bufferMap);
	/** fin **/
}


// Definition de la fonction d'affichage
GLvoid affichage(){

	Tic();
	
	// On gere le deplacement de la camera
	cam.cameraMovement();

	// Effacement du frame buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // on definit la position de la camera et ou elle regarde
   gluLookAt(cam.getcamPos().getVx(), cam.getcamPos().getVy(), cam.getcamPos().getVz(), cam.gettargetPos().getVx(), cam.gettargetPos().getVy(), cam.gettargetPos().getVz(), cam.getupWorld().getVx(), cam.getupWorld().getVy(), cam.getupWorld().getVz());

   //On dessine la map
   BuildAndDrawBuffer();
 
   //Dessin de l'océan (plan bleu)
   glBegin(GL_QUADS);
	   glColor3f(0, 0, 0.75);
	   glVertex3f(0, posOcean, 0);
	   glVertex3f(taille, posOcean, 0);
	   glVertex3f(taille, posOcean, taille);
	   glVertex3f(0, posOcean, taille);
   glEnd();

   //Mettre ça dans un VBO
   glBegin(GL_TRIANGLE_STRIP);
		glColor3f(0, 0, 0);
		//on commrnce en (0,0)
		//à droite
		for (int i = 0; i <= taille; i++)
		{
			glVertex3f(0, maMap.getHeightMap(0, i), i);
			glVertex3f(0, posOcean, i);
		}
		//en bas
		for (int i = 0; i <= taille; i++)
		{
			glVertex3f(i, maMap.getHeightMap(i, taille), taille);
			glVertex3f(i, posOcean, taille);
		}
		//à gauche
		for (int i = taille; i >= 0; i--)
		{
			glVertex3f(taille, maMap.getHeightMap(taille, i), i);
			glVertex3f(taille, posOcean, i);
		}
		//en haut
		for (int i = taille; i >= 0; i--)
		{
			glVertex3f(i, maMap.getHeightMap(i, 0), 0);
			glVertex3f(i, posOcean, 0);
		}
	glEnd();

   //Affichage écran
   glFlush();
   glutSwapBuffers();

   //Compteur FPS
   current_time = glutGet(GLUT_ELAPSED_TIME);
   ellapsed_time = current_time - last_time;
   
   if (ellapsed_time < 17)
   { 
	   Sleep(17 - ellapsed_time);
   }

   Toc();
   cout << "FPS: " << (int)(1000 / (float)ellapsed_time) << endl;
   
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
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 'o': // fil de fer
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 'i': // sommets du carre
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		case '+': //changer le seuil de l'océan
			posOcean +=1;
			break;
		case '-':
			posOcean -=1;
			break;
		case 'f':          //changer le seuil de neige
			posNeige++;
			FillDataBuffers();
			break;
		case 'v':
			posNeige--;
			FillDataBuffers();
			break;
		case 'g':			//changer le seuil de la plage
			posPlage++;
			FillDataBuffers();
			break;
		case 'b':
			posPlage--;
			FillDataBuffers();
			break;
		case 'w':
			//Ecriture du fichier OBJ pour exportation dans modeleur 3D
			cout << "Ecriture du fichier OBJ, veuillez patienter..." << endl;
			Tic();
			maMap.ecrireFichierObj();
			Toc();
			cout << "Ecriture du fichier OBJ effectuee en " << (float)ellapsed_time / 1000 << "s." << endl << endl;
			break;
		case 27:
			exit(0);
			break;
		}

		// Demande a GLUT de reafficher la scene
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
		// On remet le delt deplacement a 0
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

	Tic();
	//Initialisation Map
	maMap.initialisationAuto();
	//On génère la heightMap
	maMap.generateMatrix();
	Toc();
	cout << "Generation effectuee en " << (float)ellapsed_time / 1000 << "s." << endl;

	//Min et Max des altitudes de la carte
	float maxMin[2];
	maMap.giveMaxes(maxMin);
	cout << "Max: " << maxMin[0] << " Min: " << maxMin[1] << endl;
	cout << maxMin[0]-maxMin[1] <<endl;

	//Min et Max des altitudes de la carte
	float seuils[3];
	maMap.seuilDefinition(seuils);
	posNeige = seuils[0];
	posPlage = seuils[1];
	posOcean = seuils[2];
	cout << "neige: " << posNeige<< " plage: " << posPlage << " ocean: " << posOcean << endl<<endl;

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
   
   //Initialisation de GLU
   glewInit();

   // Définition de la couleur d'effacement du framebuffer
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   
   glEnable(GL_DEPTH_TEST);
   
   // Définition des fonctions de callbacks
   glutDisplayFunc(affichage);

	// pour que l'on puisse rester appuye sur les touches
	//glutIgnoreKeyRepeat(1);
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
