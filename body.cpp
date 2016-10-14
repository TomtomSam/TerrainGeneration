
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "heightMap.h"
#include <math.h>
#include <cstdlib>
#include <ctime>



heightMap maMap(6);


// Quelques variables globales (c'est pas bien)
GLfloat pointSize = 1.0f;

// Rotations autour de X et Y
GLint oldX = 0;
GLint oldY = 0;
GLboolean boutonClick = false;

// Taille de la fenêtre
int windowW = 1000;
int windowH = 550;
float focale = 65.0f;
float Near = 0.1f;
float Far = 100.0f;

// Déclarations des fonctions de rappel (callbacks)
GLvoid affichage();
GLvoid clavier(unsigned char touche, int x, int y);
GLvoid souris(int bouton, int etat, int x, int y);
GLvoid deplacementSouris(int x, int y);
GLvoid redimensionner(int w, int h);

void affichageTriangle(){
	int couleur = 0;
	for (int i = 0; i < pow(2, maMap.getLength()); i++)
	{
		for (int j = 0; j < pow(2, maMap.getLength()); j++)
		{
			glBegin(GL_TRIANGLES);
			switch (couleur)
			{
			case 0:
				glColor3f(1, 0, 0);
				break;
			case 1:
				glColor3f(0, 1, 0);
				break;
			case 2:
				glColor3f(1, 0, 1);
				break;
			default:
				glColor3f(1, 1, 1);
			}

			glVertex3f(i / pow(2, maMap.getLength()), maMap.getHeightMap(i, j) / maMap.getMaxHeight(), j / pow(2, maMap.getLength()));
			glVertex3f((i + 1) / pow(2, maMap.getLength()), maMap.getHeightMap(i + 1, j) / maMap.getMaxHeight(), j / pow(2, maMap.getLength()));
			glVertex3f(i / pow(2, maMap.getLength()), maMap.getHeightMap(i, j + 1) / maMap.getMaxHeight(), (j + 1) / pow(2, maMap.getLength()));

			glEnd();

			glBegin(GL_TRIANGLES);
			switch (couleur)
			{
			case 0:
				glColor3f(0, 0, 1);
				break;
			case 1:
				glColor3f(1, 1, 0);
				break;
			case 2:
				glColor3f(0, 1, 1);
				break;
			default:
				glColor3f(1, 1, 1);
			}

			glVertex3f((i + 1) / pow(2, maMap.getLength()), maMap.getHeightMap(i + 1, j + 1) / maMap.getMaxHeight(), (j + 1) / pow(2, maMap.getLength()));
			glVertex3f((i + 1) / pow(2, maMap.getLength()), maMap.getHeightMap(i + 1, j) / maMap.getMaxHeight(), j / pow(2, maMap.getLength()));
			glVertex3f(i / pow(2, maMap.getLength()), maMap.getHeightMap(i, j + 1) / maMap.getMaxHeight(), (j + 1) / pow(2, maMap.getLength()));

			glEnd();

			//Gestion Couleur
			couleur++;
			if (couleur == 3){ couleur = 0; }
		}
	}

	gluLookAt(0.5, 2, 1.5, 0.5, 0, 0.2, 0, 1, 0);

}

// Definition de la fonction d'affichage
GLvoid affichage(){
   // Effacement du frame buffer
	
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glMatrixMode(GL_MODELVIEW);
   
   glLoadIdentity();

   affichageTriangle();

   glFlush();
   glutSwapBuffers();
}

// Definition de la fonction gerant les interruptions clavier
GLvoid clavier(unsigned char touche, int x, int y) {
   
   // Suivant les touches pressees, nous aurons un comportement different de l'application
   // ESCAPE ou 'q' : fermera l'application
   // 'p' : affichage du carre plein
   // 'f' : affichage du carre en fil de fer
   // 's' : affichage des sommets du carre
   
   switch(touche) {
      case 'p' : // carre plein
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         break;
      case 'f' : // fil de fer
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         break;
      case 's' : // sommets du carre
         glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
         break;
         
         // Gestion du tampon de profondeur
      case 'd' :
		  glEnable(GL_DEPTH_TEST);
         glutPostRedisplay();
         break;
      case 'D' :
		  glDisable(GL_DEPTH_TEST);
         glutPostRedisplay();
         break;
         
      case '+':
         // Augmenter la taille des sommets affiches
         pointSize+=1.0f;
         glPointSize(pointSize);
         break;
      case '-':
         // Augmenter la taille des sommets affiches
         pointSize-=1.0f;
         if(pointSize<= 0.0f)
            pointSize = 1.0f;
         glPointSize(pointSize);
         break;
         
         
      case 'q' : // quitter
      case 27 :
         exit(0);
         break;
   }
   
   // Demande a GLUT de reafficher la scene
   glutPostRedisplay();
}

// Fonction de rappel de la souris
GLvoid souris(int bouton, int etat, int x, int y){
   // Test pour voir si le bouton gauche de la souris est appuyé
   //TODO
   
   // si on relache le bouton gauche
   // TODO
}

GLvoid deplacementSouris(int x, int y) {
   // si le bouton gauche est appuye et qu'on se deplace
   // alors on doit modifier les angles de rotations du cube
   // en fonction de la derniere position de la souris
   // et de sa position actuelle
   
   
   // Appeler le re-affichage de la scene OpenGL
   glutPostRedisplay();
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
   std::cout << "Ratio : " << ratio << std::endl;
   
   // Projection
   glMatrixMode(GL_PROJECTION);
   
   // Resetting matrix
   glLoadIdentity();
   
   // Viewport
   // // TODO Essayez de modifier l'appel à glViewport
   // en changeant les parametre d'appel a la fonction mais
   // tout en obtenant le meme resultat
   glViewport(0, 0, windowW, windowH);
   
   // Mise en place de la perspective
   // TODO : peut-on changerle ratio ici pour un meilleur resultat ?
   gluPerspective(focale, 4/3.0, Near, Far);
   
   // Placement de la caméra
   gluLookAt(0.5, 1.5, 1.5, 0.5, 0, 0.5, 0, 1, 0);
   
   // Retourne a la pile modelview
   glMatrixMode(GL_MODELVIEW);
}


int main (int argc, char *argv[])
{
	srand(time(NULL));

	//Initialisation Map
	maMap.initialisationAuto();
	maMap.generateMatrix();
	int* maxMin = maMap.giveMaxes();
	cout << "Max: " << maxMin[0] << " Min: " << maxMin[1] << endl;;

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
   
   // Définition de la couleur d'effacement du framebuffer
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   
  
   // Définition des fonctions de callbacks
   glutDisplayFunc(affichage);
   glutKeyboardFunc(clavier);
   // Nouveaux callbacks
   glutMouseFunc(souris);
   glutMotionFunc(deplacementSouris);
   glutReshapeFunc(redimensionner);
   
   // Lancement de la boucle infinie GLUT
   glutMainLoop();
   
   return 0;
}
