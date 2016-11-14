#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <GL/GL.h>
//#include <GL/glut.h>

// JPEG Lib
//#include "jpeglib.h"
//#include "jerror.h"

//Our Classes
#include "heightMap.h"
#include "vector3D.h"
#include "Point.h"

heightMap maMap(3);

// angles de rotation (coordonnees spheriques) pour calculer le vecteur vision de la camera
float angleTheta = 0.0f;
float anglePhi = 0.0f;

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

// Rotations autour de X et Y
GLint oldX = 0;
GLint oldY = 0;
GLboolean boutonClick = false;

//Gestion des couleurs
float posNeige;
float posPlage;
float posOcean;

//Mouvement caméra
float taille = maMap.getTaille();
float rayon = taille/2;

// Taille de la fenêtre
int windowW = 1000;
int windowH = 550;
float focale = 80.0f;
float Near = 0.1f;
float Far = 3000.0f;

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

//Textures
void applyTexture(){
    //Texture generation
    GLuint tex_water;
    glGenTextures(1, &tex_water);
    GLuint tex_snow;
    glGenTextures(2, &tex_snow);
    GLuint tex_grass;
    glGenTextures(3, &tex_grass);
    GLuint tex_sand;
    glGenTextures(4, &tex_sand);

    //Texture binding
    glBindTexture(GL_TEXTURE_2D, tex_water);
    glBindTexture(GL_TEXTURE_2D, tex_snow);
    glBindTexture(GL_TEXTURE_2D, tex_grass);
    glBindTexture(GL_TEXTURE_2D, tex_sand);

    //Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /*glTexParameter2(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameter2(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameter3(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameter3(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameter4(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameter4(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

    //Texture Activation
    //glActiveTexture(GL_TEXTURE1);

    //Mipmap generation
/*    glGenerateMipmap(GL_TEXTURE_2D);

    bool LoadJPEG(char* FileName, bool Fast = true);

// TEXTURE
unsigned long x;
unsigned long y;
unsigned short int bpp; //bits per pixels   unsigned short int
GLbyte* texData;             //the data of the image
GLuint ID;                //the id ogl gives it
unsigned long size;     //length of the file
int channels;      //the channels of the image 3 = RGA 4 = RGBA
GLuint type;
GLboolean colorCube = false;
GLint texEnvMode = 1;
GLboolean alpha = false;

}


bool LoadJPEG(char* FileName, bool Fast)
{
   FILE* file = fopen(FileName, "rb");  //open the file
   struct jpeg_decompress_struct info;  //the jpeg decompress info
   struct jpeg_error_mgr err;           //the error handler
   info.err = jpeg_std_error(&err);     //tell the jpeg decompression handler to send the errors to err
   jpeg_create_decompress(&info);       //sets info to all the default stuff

   //if the jpeg file doesn't load exit
   if(!file)
   {
      fprintf(stderr, "Error reading JPEG file %s!!!", FileName);
      return false;
   }

   jpeg_stdio_src(&info, file);    //tell the jpeg lib the file we are reading
   jpeg_read_header(&info, TRUE);   //tell it to start reading it

   //if it wants to be read fast or not
   if(Fast)
   {
      info.do_fancy_upsampling = FALSE;
   }

   jpeg_start_decompress(&info);    //decompress the file

   //set the x and y
   x = info.output_width;
   y = info.output_height;
   channels = info.num_components;

   type = GL_RGB;

   if(channels == 4)
   {
      type = GL_RGBA;
   }

   bpp = channels * 8;

   size = x * y * 3;

   //read turn the uncompressed data into something ogl can read
   texData = new GLbyte[size];      //setup data for the data its going to be handling

   GLbyte* p1 = texData;
   GLbyte** p2 = &p1;
   int numlines = 0;

   while(info.output_scanline < info.output_height)
   {
      numlines = jpeg_read_scanlines(&info, (JSAMPLE**)p2, 1);
      *p2 += numlines * 3 * info.output_width;
   }

   jpeg_finish_decompress(&info);   //finish decompressing this file

   fclose(file);                    //close the file

   return true;*/
}


void affichageTriangle(){
	int couleur = 0;
	int taille=pow(2, maMap.getLength());
	vector<float>  color;
	for (int i = 0; i < pow(2, maMap.getLength()); i++)
	{
		for (int j = 0; j < pow(2, maMap.getLength()); j++)
		{
			glBegin(GL_TRIANGLES);

			float color[3];

			maMap.vertexColor(maMap.getHeightMap(i, j), posNeige, posPlage, posOcean, color);
			glColor3f(maMap[i][j].getR(),maMap[i][j].getG(),maMap[i][j].getB());
			glVertex3f(i, maMap.getHeightMap(i, j), j);

			maMap.vertexColor(maMap.getHeightMap(i + 1, j), posNeige, posPlage, posOcean, color);
			glColor3f(maMap[i][j].getR(),maMap[i][j].getG(),maMap[i][j].getB());
			glVertex3f((i + 1), maMap.getHeightMap(i + 1, j), j);

			maMap.vertexColor(maMap.getHeightMap(i, j), posNeige, posPlage, posOcean, color);
			glColor3f(maMap[i][j].getR(),maMap[i][j].getG(),maMap[i][j].getB());
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

// Definition de la fonction d'affichage
GLvoid affichage(){
   // Effacement du frame buffer

	// On gere le deplacement de la camera
	cameraMovement(deltaMove, deltaStrafe);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // on definit la position de la camera et ou elle regarde
   gluLookAt(camPos.getVx(), camPos.getVy(), camPos.getVz(), targetPos.getVx(), targetPos.getVy(), targetPos.getVz(), upWorld.getVx(), upWorld.getVy(), upWorld.getVz());

   affichageTriangle();

   glFlush();
   glutSwapBuffers();
}

// Definition de la fonction gerant les interruptions clavier
GLvoid clavier(unsigned char touche, int x, int y) {
	switch (touche) {
			// Q et D on strafe
		case 'q':
		case 'Q':
			deltaStrafe += moveSensitivity;
			cameraMovement(0, deltaStrafe);
			break;
		case 'd':
		case 'D':
			deltaStrafe -= moveSensitivity;
			cameraMovement(0, deltaStrafe);
			break;

			// Z et S avance/recule
		case 'z':
		case 'Z':
			deltaMove += moveSensitivity;
			cameraMovement(deltaMove, 0);
			break;
		case 's':
		case 'S':
			deltaMove -= moveSensitivity;
			cameraMovement(deltaMove, 0);
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
+			posOcean++;
			break;
		case '-':
			posOcean--;
			break;
		case 'f':          //changer le seuil de neige
			posNeige++;
			break;
		case 'v':
			posNeige--;
			break;
		case 'g':			//changer le seuil de la plage
			posPlage++;
			break;
		case 'b':
			posPlage--;
			break;
		case 27:
			exit(0);
			break;
		}

		// Demande a GLUT de reafficher la scene
		glutPostRedisplay();
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
		   else
		   {
			   // state = GLUT_DOWN
			   // si l'on a clique sur le bouton gauche
			   // on garde les positions de la souris au moment du clic gauche
			   xOrigin = x;
			   yOrigin = y;
		   }
	   }
   }



GLvoid deplacementSouris(int x, int y) {

	// On ne fait quelque chose que si l'utilisateur
	// a deja clique quelque part avec le bouton gauche
	if (xOrigin >= 0 || yOrigin >= 0) {

		//Definition des deltas des angles
		deltaTheta = (x - xOrigin)*mouseRotSensitivity;
		deltaPhi = (y - yOrigin)*mouseRotSensitivity;

		// Calcul du nouveau vecteur vision :
		forwardView.setVx(cos(anglePhi + deltaPhi)*sin(angleTheta + deltaTheta));
		forwardView.setVy(-sin(anglePhi + deltaPhi));
		forwardView.setVz(-cos(anglePhi + deltaPhi)*cos(angleTheta + deltaTheta));

		forwardMove = forwardView;
		forwardMove.setVy(0.0f);

		// En se basant sur les coordonnes spheriques
		// normalisation du vecteur forward
		forwardView.normalize();
		forwardMove.normalize();

		// Up ne change pas
		// et right est le cross product entre up et forward
		rightView = upWorld.crossProduct(forwardView);
		rightMove = upWorld.crossProduct(forwardMove);

		// Mettre a jour la cible = point "vise" par la camera
		targetPos = camPos + forwardView;

		// Pour le mode "FPS" on force les deplacements a avoir lieu sur le plan de la camera !
		// Mise a jour des vecteurs forwardMove et rightMove

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

   // on definit la position de la camera et ou elle regarde
   gluLookAt(camPos.getVx(), camPos.getVy(), camPos.getVz(), targetPos.getVx(), targetPos.getVy(), targetPos.getVz(), upWorld.getVx(), upWorld.getVy(), upWorld.getVz());


}

int main (int argc, char *argv[])
{
	srand(time(NULL));

	//Initialisation Map
	maMap.initialisationAuto();
	maMap.generateMatrix();
	maMap.mapColor(float snow, float beach, float water);

	//Ecriture du fichier OBJ pour exportation dans modeleur 3D
	maMap.ecrireFichierObj();

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

   glEnable(GL_DEPTH_TEST);

   // on initialise la position de la camera
   camPos = Vector3D(0, 1, 5);

   // on initialise les vecteurs 'view'
   forwardView = Vector3D(0, 0, -1);
   upWorld = Vector3D(0, 1, 0);
   rightView = Vector3D(-1, 0, 0);

   // Pour le FPS mode
   forwardMove = Vector3D(0, 0, -1);
   rightMove = Vector3D(-1, 0, 0);

   // on initialise la cible a partir de la camera et du vecteur vision
   targetPos = camPos + forwardView;

   // Initialisation des "constantes"
   moveSensitivity = 0.1f;
   mouseRotSensitivity = 0.01f;

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
