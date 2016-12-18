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
#include "Program.h"
#include "CallbackHandler.h"


// Initialisation de la carte
heightMap maMap(5);

//// Creation des textures
//Texture water;
//Texture grass;
//Texture ice;
//Texture sand;
//Texture snow;

// Creation du chrono
Chrono chrono;

// Initialisation du Vertex Buffer Object
VBO monVBO;

//// Initialisation des shaders
//Shader VS, FS, bumpVS, bumpFS; 
//Program prog, bumpprog;

// Creation et initialisation  de la camera
FreeFlyCamera camera(static_cast<float>(maMap.getTaille()) / 100, 0.5*maMap.getTaille(), maMap.getTaille(), -0.5*maMap.getTaille(), 0, -0.5*maMap.getTaille(), 0.75*maMap.getTaille());

// Initialisation des proprietes de la fenêtre
int windowW = 1000;
int windowH = 550;
int FPS = 0;

// Initialisation de l'interface utilisateur
InterfaceUtilisateur IU(&maMap, &monVBO, &camera);


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

	// Dessin de l'interface graphique
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	/*if(IU.getRenderMode()<3){prog.useProgram();}
	else{glUseProgram(0);}*/
	glPolygonMode(GL_FRONT_AND_BACK, maMap.getRenderMode());
	monVBO.DrawBuffer();
	//glUseProgram(0);

	// Dessin de l'ocean
	//if(IU.getRenderMode()<4){prog.useProgram();}
	//else{glUseProgram(0);}
	maMap.dessinOcean();
	if (IU.getIsClicked())
	{
		Curseur* curseur = dynamic_cast<Curseur*>(IU.getObjet(IU.getIndexObject()));
		if (IU.getIndexObject() == 7)//Ocean
		{
			maMap.dessinOceanPreview(curseur->getValue());
		}
		else if (IU.getIndexObject() == 8)//Dilatation
		{
			maMap.dessinDilatationPreview(curseur->getValue());
		}
		
	}
	//glUseProgram(0);

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

	// Definition de la couleur d'effacement du framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Activation du Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Affichage du dessus de la map uniquement
	glPolygonMode(GL_BACK, GL_FILL);

	//// Chargement des textures
	//water.loadTexture("WATER.jpg");
	//sand.loadTexture("SAND.jpg");
	//ice.loadTexture("ICE.jpg");
	//grass.loadTexture("GRASS.jpg");
	//snow.loadTexture("SNOW.jpg");

	// Initialisation de la map
	maMap.initialisationAuto();

	// Generation de la map
	maMap.generateMatrix();

	// Remplissage des vecteurs de data a envoyer au GPU
	maMap.FillDataBuffersPosColorsTex();

	// Remplissage du VBO
	monVBO.FeedCol(maMap.getCol());
	monVBO.FeedPos(maMap.getPos());
	monVBO.FeedTex(maMap.getTex());
	monVBO.BuildBuffer();

	float maxes[2];
	maMap.giveMaxes(maxes);
	//Initialisation des pos de curseur
	//Curseur Océan initialisé à posOcéan
	int posCurseurIni = 200 * (maMap.getPosOcean() - maxes[1]) / (maxes[0] - maxes[1]);
	Curseur* curseur = dynamic_cast<Curseur*>(IU.getObjet(7));
	curseur->setPosCurseur(posCurseurIni);
	//Curseur de dilatation initialisé à 1.0f
	curseur = dynamic_cast<Curseur*>(IU.getObjet(8));
	curseur->setPosCurseur(0);

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

	//// Creation des shaders et du program associe a la map
	//VS.loadShader("vertexShader.vert", GL_VERTEX_SHADER);
	//FS.loadShader("fragmentShader.frag", GL_FRAGMENT_SHADER);
	//prog.createProgram();
	//prog.addShaderToProgram(&VS);
	//prog.addShaderToProgram(&FS);
	//prog.linkProgram(&VS, &FS);

	//// Creation des shaders et du program associe au bump mapping
	//bumpVS.loadShader("vertexBump.vert", GL_VERTEX_SHADER);
	//bumpFS.loadShader("fragmentBump.frag", GL_FRAGMENT_SHADER);
	//bumpprog.createProgram();
	//bumpprog.addShaderToProgram(&bumpVS);
	//bumpprog.addShaderToProgram(&bumpFS);
	//bumpprog.linkProgram(&bumpVS, &bumpFS);

	//// Assignation des variables des shaders (reglages multitexturing)
	//prog.useProgram();
	//float maxMin[2];
	//maMap.giveMaxes(maxMin);
	//float oc = maMap.getPosOcean();
	//float dis = maxMin[0]-oc;

	//prog.setUniformf("maxHeight", maxMin[0]);
	//prog.setUniformf("minHeight", maxMin[1]);
	//prog.setUniformf("beachInf", oc);
	//prog.setUniformf("beachSup", maMap.getPosOcean()+dis*0.05);
	//prog.setUniformf("grassInf", maMap.getPosOcean()+dis*0.02);
	//prog.setUniformf("grassSup", maMap.getPosOcean()+dis*0.82);
	//prog.setUniformf("snowInf", maMap.getPosOcean()+dis*0.8);
	//prog.setUniformf("snowSup", maMap.getPosOcean()+dis*87);
	//prog.setUniformf("iceInf", maMap.getPosOcean()+dis*0.85);

	//GLint texture, texture1, texture2, texture3, texture4;
	//
	//bumpprog.useProgram();
	//glActiveTexture(GL_TEXTURE0 + water.getTexture());
	//glBindTexture(GL_TEXTURE_2D, water.getTexture());
	//texture = glGetUniformLocation(bumpprog.getProgramID(), "bumptex");
	//glUniform1i(texture, water.getTexture());
	//bumpprog.setUniformi("bumpID", water.getTexture());
	//	
	//prog.useProgram();
	//glActiveTexture(GL_TEXTURE0 + sand.getTexture());
	//glBindTexture(GL_TEXTURE_2D, sand.getTexture()); 
	//texture1 = glGetUniformLocation(prog.getProgramID(), "tex_sand");
	//glUniform1i(texture1, sand.getTexture());
	//glActiveTexture(GL_TEXTURE0 + ice.getTexture());
	//glBindTexture(GL_TEXTURE_2D, ice.getTexture()); 
	//texture2 = glGetUniformLocation(prog.getProgramID(), "tex_ice");
	//glUniform1i(texture2, ice.getTexture());
	//glActiveTexture(GL_TEXTURE0 + grass.getTexture());
	//glBindTexture(GL_TEXTURE_2D, grass.getTexture());
	//texture3 = glGetUniformLocation(prog.getProgramID(), "tex_grass");
	//glUniform1i(texture3, grass.getTexture());
	//glActiveTexture(GL_TEXTURE0 + snow.getTexture());
	//glBindTexture(GL_TEXTURE_2D, snow.getTexture());
	//texture4 = glGetUniformLocation(prog.getProgramID(), "tex_snow");
	//glUniform1i(texture4, snow.getTexture());
	//glUseProgram(0);

	// Lancement de la boucle infinie GLUT
	glutMainLoop();

	return 0;
}