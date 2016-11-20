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

heightMap maMap(5);


int last_time = glutGet(GLUT_ELAPSED_TIME);
int current_time, ellapsed_time;

// angles de rotation (coordonnees spheriques) pour calculer le vecteur vision de la camera
float angleTheta = 0.825f;
float anglePhi = -1.86f;

// Difference "Fly" vs. "FPS" modes
Vector3D forwardMove;
Vector3D rightMove;

// Camera viewing vectors
Vector3D forwardView;
Vector3D rightView;
// On garde le vecteur "up" du monde : 0 1 0
Vector3D upWorld;

// Camera position
Vector3D camPos;
Vector3D targetPos;

// Sensibilite de deplacement
float moveSensitivity;
float mouseRotSensitivity;

// les increments des angles theta et phi
// egales a 0 quand on n'appuie sur aucune touche
float deltaTheta = 0.0f;
float deltaPhi = 0.0f;

// increments de deplacement
// remis a 0 quand on appuis sur rien
float deltaMove = 0;
float deltaStrafe = 0.0f;

// coordonnes de la souris au moment du clic gauche
int xOrigin = -1;
int yOrigin = -1;

//liste d'affichage
//GLuint list;
GLuint bufferMap;
#define BUFFER_OFFSET(a) ((char*)NULL + (a))

//Gestion des couleurs
float posNeige;
float posPlage;
float posOcean;

//Mouvement caméra
int taille = pow(2, maMap.getLength());
const int TAILLE = taille;
const int NB_VERTS = pow(TAILLE + 1, 2);

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


// Fonction de gestion du deplacement de la camera
void cameraMovement(float dM, float dS) {
	
	camPos = camPos + dS*rightView + dM*forwardView;
	// Mettre a jour la cible
	targetPos = camPos + forwardView;
}


vector<float> pos;
vector<float> colors;
float color[3];

void FillDataBuffers()
{

	current_time = glutGet(GLUT_ELAPSED_TIME);
	last_time = current_time;

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

	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
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


/*
void affichageTriangle(){
	//vector<float>  color;
	for (int i = 0; i < pow(2, maMap.getLength()); i++)
	{
		for (int j = 0; j < pow(2, maMap.getLength()); j++)
		{
			glBegin(GL_TRIANGLES);

			float color[3];

			maMap.vertexColor(maMap.getHeightMap(i, j), posNeige, posPlage, posOcean, color);
			glColor3f(color[0], color[1], color[2]);
			glVertex3f(i, maMap.getHeightMap(i, j), j);

			maMap.vertexColor(maMap.getHeightMap(i + 1, j), posNeige, posPlage, posOcean, color);
			glColor3f(color[0], color[1], color[2]);
			glVertex3f((i + 1), maMap.getHeightMap(i + 1, j), j);

			maMap.vertexColor(maMap.getHeightMap(i, j), posNeige, posPlage, posOcean, color);
			glColor3f(color[0], color[1], color[2]);
			glVertex3f(i, maMap.getHeightMap(i, j + 1), (j + 1));

			glEnd();

			glBegin(GL_TRIANGLES);

			maMap.vertexColor(maMap.getHeightMap(i + 1, j + 1), posNeige, posPlage, posOcean, color);
			glColor3f(color[0], color[1], color[2]);
			glVertex3f((i + 1), maMap.getHeightMap(i + 1, j + 1), (j + 1));
			
			maMap.vertexColor(maMap.getHeightMap(i + 1, j), posNeige, posPlage, posOcean, color);
			glColor3f(color[0], color[1], color[2]);
			glVertex3f((i + 1), maMap.getHeightMap(i + 1, j), j);

			maMap.vertexColor(maMap.getHeightMap(i, j + 1), posNeige, posPlage, posOcean, color);
			glColor3f(color[0], color[1], color[2]);
			glVertex3f(i, maMap.getHeightMap(i, j + 1), (j + 1));

			glEnd();

		}
	}

	//Dessin de l'océan (plan bleu)
	glBegin(GL_QUADS);
			
			glColor3f(0,0,0.75);
			glVertex3f(0		, posOcean		, 0			);
			glVertex3f(taille	, posOcean		, 0			);
			glVertex3f(taille	, posOcean		, taille	);
			glVertex3f(0		, posOcean		, taille	);

	glEnd();
}
*/

// Definition de la fonction d'affichage
GLvoid affichage(){

	current_time = glutGet(GLUT_ELAPSED_TIME);
	last_time = current_time;
	
   // Effacement du frame buffer
	
	// On gere le deplacement de la camera
	cameraMovement(deltaMove, deltaStrafe);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // on definit la position de la camera et ou elle regarde
   gluLookAt(camPos.getVx(), camPos.getVy(), camPos.getVz(), targetPos.getVx(), targetPos.getVy(), targetPos.getVz(), upWorld.getVx(), upWorld.getVy(), upWorld.getVz());


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

   current_time = glutGet(GLUT_ELAPSED_TIME);
   ellapsed_time = current_time - last_time;
   cout << "FPS: " << (int)(1000 / (float)ellapsed_time) << endl;
   
}

// Definition de la fonction gerant les interruptions clavier
GLvoid clavier(unsigned char touche, int x, int y) {
	switch (touche) {
			// Q et D on strafe
		case 'q':
		case 'Q':
			deltaStrafe += moveSensitivity;		
			break;
		case 'd':
		case 'D':
			deltaStrafe -= moveSensitivity;	
			break;
			// Z et S avance/recule
		case 'z':
		case 'Z':
			deltaMove += moveSensitivity;
			break;
		case 's':
		case 'S':
			deltaMove -= moveSensitivity;
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
		deltaStrafe = 0;
		break;

		// On arrete d'avance/reculer
	case 'z':
	case 's':
	case 'Z':
	case 'S':
		deltaMove = 0;
		break;
	}
}


// Fonction de gestion du clavier (touche speciale appuyee)
void specialKeyDown(int key, int xx, int yy) {

	switch (key) {
		// Deplacement avance/recul
	case GLUT_KEY_UP:
		deltaMove++;
		break;
	case GLUT_KEY_DOWN:
		deltaMove--;
		break;

		// Strafe a gauche/droite
	case GLUT_KEY_RIGHT:
		deltaStrafe++;
		break;
	case GLUT_KEY_LEFT:
		deltaStrafe--;
		break;
	default:
		break;
	}

}

// Fonction de gestion du clavier (touche speciale relachee)
void releaseSpecialKey(int key, int x, int y) {

	switch (key) {
		// On remet le delt deplacement a 0
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		deltaMove = 0;
		break;

		// On remet le delta strafe a 0
	case GLUT_KEY_RIGHT:
	case GLUT_KEY_LEFT:
		deltaStrafe = 0;
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
		   if (etat == GLUT_UP) {
			   angleTheta += deltaTheta;
			   anglePhi += deltaPhi;
			   xOrigin = -1;
			   yOrigin = -1;
		   }
		   else  {// state = GLUT_DOWN
			   // si l'on a clique sur le bouton gauche
			   // on garde les positions de la souris au moment du clic gauche
			   xOrigin = x;
			   yOrigin = y;
		   }
	   }
   }



   // Fonction de gestion du deplacement de la souris
   void deplacementSouris(int x, int y) {

	   // On ne fait quelque chose que si l'utilisateur
	   // a deja clique quelque part avec le bouton gauche
	   if (xOrigin >= 0 || yOrigin >= 0) {

		   // mise a jour des deltas des angles theta et phi
		   deltaTheta = (x - xOrigin)*mouseRotSensitivity;
		   deltaPhi = (y - yOrigin)*mouseRotSensitivity;

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

		   // Demande a GLUT de reafficher la scene
		   glutPostRedisplay();
	   }
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

	//Initialisation Map
	maMap.initialisationAuto();

	//On génère la heightMap
	maMap.generateMatrix();
	
	//Ecriture du fichier OBJ pour exportation dans modeleur 3D
	maMap.ecrireFichierObj();
	
	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
	cout << "generation effectuee en " << (float)ellapsed_time/1000 << "s." << endl;

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
	cout << "neige: " << posNeige<< " plage: " << posPlage << " ocean: " << posOcean << endl;

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

   // on initialise la position de la camera
   camPos = Vector3D(1.5*taille, taille/2, 1.5*taille);

   // on initialise les vecteurs 'view'
   forwardView = Vector3D(-1.5*taille, -taille/2, -1.5*taille);
   upWorld = Vector3D(0, 1, 0);
   rightView = upWorld.crossProduct(forwardView);

   // Pour le FPS mode
   forwardMove = forwardView;
   rightMove = rightView;

   // on initialise la cible a partir de la camera et du vecteur vision
   targetPos = camPos + forwardView;

   // Initialisation des "constantes"
   moveSensitivity = (float)taille/100;
   mouseRotSensitivity = 0.005f;

   
   // Définition des fonctions de callbacks
   glutDisplayFunc(affichage);

   // pour que l'on puisse rester appuye sur les touches
   //glutIgnoreKeyRepeat(1);
   glutKeyboardFunc(clavier);
   glutKeyboardUpFunc(clavierUp);
   glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(releaseSpecialKey);

   // Nouveaux callbacks
   glutMouseFunc(souris);
   glutMotionFunc(deplacementSouris);
   glutReshapeFunc(redimensionner);
   
   // Lancement de la boucle infinie GLUT
   glutMainLoop();
   
   return 0;
}
