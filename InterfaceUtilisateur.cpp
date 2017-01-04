#include "InterfaceUtilisateur.h"

// CONSTRUCTOR
InterfaceUtilisateur::InterfaceUtilisateur(heightMap* _maMap, VBO* _monVBO,FreeFlyCamera* _maCamera)
{
	// Initialisation des attributs
	maMap = _maMap;
	monVBO = _monVBO;
	maCamera=_maCamera;
	indexObject = 0;
	survolObject = false;
	isClicked = false;
	renderMode = 0;

	// Creation de l'interface
	ajouterObjet(new Label(820, 40, "Map Size"));		//0
	ajouterObjet(new Bouton(675, 40, "-"));				//1
	ajouterObjet(new Label(620, 40, "5"));				//2
	ajouterObjet(new Bouton(565, 40, "+"));				//3
	ajouterObjet(new Bouton(510, 40, "Generate"));		//4
	ajouterObjet(new Bouton(375, 40, "Render Mode"));	//5
	ajouterObjet(new Bouton(200, 40, "Export OBJ"));	//6
	ajouterObjet(new Curseur(220, 65));					//7
	ajouterObjet(new Curseur(220, 100));				//8
	ajouterObjet(new Label(345, 79, "Ocean "));			//9
	ajouterObjet(new Label(345, 114, "Expand"));		//10
	
}

// DESTRUCTOR
InterfaceUtilisateur::~InterfaceUtilisateur(){}

// GETTERS
bool InterfaceUtilisateur::getSurvol(){ return survolObject; }
string InterfaceUtilisateur::getLabel(int i){ return objetsIU[i]->getLabel(); }
int InterfaceUtilisateur::getRenderMode(){ return renderMode; }
bool InterfaceUtilisateur::getIsClicked(){ return isClicked; }
void InterfaceUtilisateur::setIsClicked(bool _isClicked){ isClicked = _isClicked; }
IUobject* InterfaceUtilisateur::getObjet(int i){ return objetsIU[i]; }
int InterfaceUtilisateur::getIndexObject(){ return indexObject; }

// METHODS
void InterfaceUtilisateur::ajouterObjet(IUobject* newObj)
{
	objetsIU.push_back(newObj);
}

// Detection du bouton survolé
void InterfaceUtilisateur::testSurvol(int x, int y,  int windowW, int windowH)
{
	unsigned int index = 0;
	survolObject = false;

	// Detection de l'indice du bouton survole
	while (survolObject == false && index < objetsIU.size())
	{
		//Réalisation du test de survol sur les objets activables
		if (objetsIU[index]->getTag() != "Label")
		{
			survolObject = objetsIU[index]->isMouseInMe(x, y, windowW, windowH);
		}
		index++;
	}

	// Assignation a indexBut de l'indice du bouton survole
	if (survolObject){ indexObject = index - 1; }
}

// Dessin de l'interface utilisateur
void InterfaceUtilisateur::draw(int windowW, int windowH)
{
	// Affichage des objets de l'IU
	for (unsigned int i = 0; i < objetsIU.size();i++)
	{
		objetsIU[i]->draw(windowW,windowH);
	}
}

// Reduction de la taille de la heightmap
void InterfaceUtilisateur::reductionTaille()
{
	// Recuperation du contenu du label taille
	string taille = objetsIU[2]->getLabel();

	// Convertion du label taille en entier
	int Taille = stoi(taille, nullptr);

	// Reduction de la taille
	if (Taille > 1)
	{ 
		Taille--; 
		taille = to_string(Taille);
		objetsIU[2]->setLabel(taille);
	}
}

// Augmentation de la taille de la heightmap
void InterfaceUtilisateur::augmentationTaille()
{
	// Recuperation du contenu du label taille
	string taille = objetsIU[2]->getLabel();

	// Convertion du label taille en entier
	int Taille = stoi(taille, nullptr);

	// Augmentation de la taille
	if (Taille <11)
	{
		Taille++;
		taille = to_string(Taille);
		objetsIU[2]->setLabel(taille);
	}
}

// Generation d'une nouvelle heightmap
void InterfaceUtilisateur::generation()
{
	// Chargement de la taille de la map
	string taille = objetsIU[2]->getLabel();
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

	//Initialisation de la Map
	maMap->initialisationAuto();

	// Generation de la map
	maMap->generateMatrix();

	// Remplissage des vecteurs de data à envoyer au GPU
	maMap->FillDataBuffersPosColorsTex();

	// Remplissage du VBO
	monVBO->FeedCol(maMap->getCol());
	monVBO->FeedPos(maMap->getPos());
	monVBO->FeedTex(maMap->getTex());
	monVBO->BuildBuffer();

	float maxes[2];
	maMap->giveMaxes(maxes);
	//Initialisation des pos de curseur
	//Curseur Océan
	int posCurseurIni = 200 * (maMap->getPosOcean() - maxes[1]) / (maxes[0] - maxes[1]);
	Curseur* curseur = dynamic_cast<Curseur*>(objetsIU[7]);
	curseur->setPosCurseur(posCurseurIni);
	//curseur de dilatation
	curseur = dynamic_cast<Curseur*>(objetsIU[8]);
	curseur->setPosCurseur(0);
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

void InterfaceUtilisateur::modifyOcean()
{
	float maxes[2];
	maMap->giveMaxes(maxes);
	float increment = (maxes[0] - maxes[1]) / 200;

	if (isClicked)
	{
		//Curseur actif
		//Prévisualisation de la position de l'océan
		Curseur* curseur = dynamic_cast<Curseur*>(objetsIU[indexObject]);
		curseur->setValue(maxes[1]+curseur->getPosCurseur()*increment);
	}
	else
	{	
		//Curseur inactif
		//Réalisation des modifications
		Curseur* curseur = dynamic_cast<Curseur*>(objetsIU[indexObject]);
		maMap->setPosOcean(curseur->getValue());
		monVBO->FeedCol(maMap->getCol());
		monVBO->FeedPos(maMap->getPos());
		monVBO->ActualizeColBuffer();
		monVBO->ActualizePosBuffer();
	}

}

void InterfaceUtilisateur::modifyDilatation()
{
	float value = 0.02;

	if (isClicked)
	{
		//Curseur actif
		//Prévisualisation de la dilatation
		Curseur* curseur = dynamic_cast<Curseur*>(objetsIU[indexObject]);
		curseur->setValue(1+curseur->getPosCurseur()*value);
	}
	else
	{	
		//Curseur Inactif
		//Réalisation des modifications
		Curseur* curseur = dynamic_cast<Curseur*>(objetsIU[indexObject]);
		maMap->setDilatation(curseur->getValue());
		monVBO->FeedCol(maMap->getCol());
		monVBO->FeedPos(maMap->getPos());
		monVBO->ActualizeColBuffer();
		monVBO->ActualizePosBuffer();
	}

}
// Gestion des appuis sur les boutons
void InterfaceUtilisateur::action()
{
	// Choix l'action en fonction du bouton selectionne
	switch (indexObject)
	{
	// Bouton selectionne : "-"
	case 1 :
		reductionTaille();
		break;

	// Bouton selectionne : "+" 
	case 3 :
		augmentationTaille();
		break;

	// Bouton  selectionne : "Generate" 
	case 4 :
		generation();
		break;

	// Bouton  selectionne : "Render Mode" 
	case 5 :
		renderModeSelection();
		break;

	// Bouton  selectionne : "Export OBJ" 
	case 6 :
		maMap->ecrireFichierObj();
		break;

	// Curseur Position Océan
	case 7:
		modifyOcean();
		break;

	//Cursuer Dilatation
	case 8:
		modifyDilatation();
		break;

	// Cas par defaut
	default:
		break;
	}
}