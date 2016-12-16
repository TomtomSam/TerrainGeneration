#include "InterfaceUtilisateur.h"

// CONSTRUCTOR
InterfaceUtilisateur::InterfaceUtilisateur(heightMap* _maMap, VBO* _monVBO,FreeFlyCamera* _maCamera)
{
	// Initialisation des attributs
	maMap = _maMap;
	monVBO = _monVBO;
	maCamera=_maCamera;
	indexBut = 0;
	survolBouton = false;
	renderMode = 0;

	// Creation de l'interface
	ajouterLabel(new Label(820, 40, "Map Size"));
	ajouterBouton(new Bouton(675, 40, "-"));
	ajouterLabel(new Label(620, 40, "5"));
	ajouterBouton(new Bouton(565, 40, "+"));
	ajouterBouton(new Bouton(510, 40, "Generate"));
	ajouterBouton(new Bouton(375, 40, "Render Mode"));
	ajouterBouton(new Bouton(200, 40, "Export OBJ"));
}

// DESTRUCTOR
InterfaceUtilisateur::~InterfaceUtilisateur(){}

// GETTERS
bool InterfaceUtilisateur::getSurvol(){ return survolBouton; }
Label* InterfaceUtilisateur::getLabel(int i){ return labels[i]; }
int InterfaceUtilisateur::getRenderMode(){ return renderMode; }

// METHODS
// Ajout d'un bouton a l'interface utilisateur
void InterfaceUtilisateur::ajouterBouton(Bouton* newBut)
{
	boutons.push_back(newBut);
}

// Ajout d'un label a l'interface utilisateur
void InterfaceUtilisateur::ajouterLabel(Label* newLab)
{
	labels.push_back(newLab);
}

// Detection du bouton survole
void InterfaceUtilisateur::testSurvol(int x, int y,  int windowW, int windowH)
{
	unsigned int index = 0;
	survolBouton = false;

	// Detection de l'indice du bouton survole
	while (survolBouton == false && index < boutons.size())
	{
		survolBouton = boutons[index]->isMouseInMe(x, y, windowW, windowH);
		index++;
	}

	// Assignation a indexBut de l'indice du bouton survole
	if (survolBouton){ indexBut = index - 1; }
}

// Dessin de l'interface utilisateur
void InterfaceUtilisateur::draw(int windowW, int windowH)
{
	// Affichage des boutons
	for (unsigned int i = 0; i < boutons.size();i++)
	{
		boutons[i]->draw(windowW,windowH);
	}

	// Affichage des labels
	for (unsigned int i = 0; i < labels.size(); i++)
	{
		labels[i]->draw(windowW, windowH);
	}
}

// Reduction de la taille de la heightmap
void InterfaceUtilisateur::reductionTaille()
{
	// Recuperation du contenu du label taille
	string taille = labels[1]->getLabel();

	// Convertion du label taille en entier
	int Taille = stoi(taille, nullptr);

	// Reduction de la taille
	if (Taille > 1)
	{ 
		Taille--; 
		taille = to_string(Taille);
		labels[1]->setLabel(taille);
	}
}

// Augmentation de la taille de la heightmap
void InterfaceUtilisateur::augmentationTaille()
{
	// Recuperation du contenu du label taille
	string taille = labels[1]->getLabel();

	// Convertion du label taille en entier
	int Taille = stoi(taille, nullptr);

	// Reduction de la taille
	if (Taille <11)
	{
		Taille++;
		taille = to_string(Taille);
		labels[1]->setLabel(taille);
	}
}

// Generation d'une nouvelle heightmap
void InterfaceUtilisateur::generation()
{
	// Chargement de la taille de la map
	string taille = labels[1]->getLabel();
	int Taille = stoi(taille, nullptr);
	maMap->setLength(Taille);
	maMap->setTaille();

	// Destruction des buffers existants
	monVBO->DestroyVBO();

	// Reinitialisation de la camera
	maCamera->setcamPos(0.5*maMap->getTaille(), maMap->getTaille(), -0.5*maMap->getTaille());
	maCamera->settargetPos(0, -0.5*maMap->getTaille(), 0.75*maMap->getTaille());
	maCamera->setMoveSensitivity(static_cast<float>(maMap->getTaille()) / 100);
	maCamera->setFar(static_cast<float>(maMap->getTaille() * 2));
	maCamera->resetParam();

	// Initialisation de la map
	maMap->setWidth(maMap->getLength());
	maMap->setMaxDepth(static_cast<float>(-pow(2, maMap->getLength())));
	maMap->setMaxHeight(static_cast<float>(pow(2, maMap->getLength())));
	maMap->resetDilatation();
	maMap->clearMatrix();

	//Initialisation de la matrice
	for (int l = 0; l<1 + pow(2, maMap->getLength()); l++)
	{
		vector<Point *> row;
		for (int c = 0; c<1 + pow(2, maMap->getLength()); c++)
		{
			row.push_back(new Point(0, 0, 0));
		}
		maMap->setMatrix(row);
	}

	maMap->initialisationAuto();

	// Generation de la map
	maMap->generateMatrix();

	// Remplissage des vecteurs de data a envoyer au GPU
	maMap->FillDataBuffersPosColorsTex();

	// Remplissage du VBO
	monVBO->FeedCol(maMap->getCol());
	monVBO->FeedPos(maMap->getPos());
	monVBO->FeedTex(maMap->getTex());
	monVBO->BuildBuffer();
}

void InterfaceUtilisateur::renderModeSelection()
{
	// Gestion du changement de mode
	renderMode++;
	if (renderMode == 5){ renderMode = 0; }

	// Gestion des instructions associees aux differents modes
	switch (renderMode)
	{
	// Affichage des polygones pleins
	case 0 :
		maMap->setRenderMode(GL_FILL);
		break;

	// Affichage en mode fil de fer
	case 1 :
		maMap->setRenderMode(GL_LINE);
		break;

	// Affichage en nuage de points
	case 2 :
		maMap->setRenderMode(GL_POINT);
		break;
	// Affichage sans le shader de la heightmap
	case 3 :
		maMap->setRenderMode(GL_FILL);
		break;
	// Affichage sans le shader de l'ocean
	case 4 :
		break;
	// Cas par defaut
	default :
		break;
	}
}

// Gestion des appuis sur les boutons
void InterfaceUtilisateur::action()
{
	// Choix l'action en fonction du bouton selectionne
	switch (indexBut)
	{
	// Bouton selectionne : "-"
	case 0 :
		reductionTaille();
		break;

	// Bouton selectionne : "+" 
	case 1 :
		augmentationTaille();
		break;

	// Bouton  selectionne : "Generate" 
	case 2 :
		generation();
		break;

	// Bouton  selectionne : "Render Mode" 
	case 3 :
		renderModeSelection();
		break;

	// Bouton  selectionne : "Export OBJ" 
	case 4 :
		maMap->ecrireFichierObj();
		break;

	// Cas par defaut
	default:
		break;
	}
}