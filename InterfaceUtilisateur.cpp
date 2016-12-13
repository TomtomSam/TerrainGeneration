#include "InterfaceUtilisateur.h"


InterfaceUtilisateur::InterfaceUtilisateur(heightMap* _maMap, VBO* _monVBO,FreeFlyCamera* _maCamera)
{
	//Initialisation des attributs
	maMap = _maMap;
	monVBO = _monVBO;
	maCamera=_maCamera;
	indexBut = 0;
	survolBouton = false;
	renderMode = 0;

	//Création de l'interface

	//Configuration verticale
	/*ajouterLabel(new Label(200, 50, "Map Size"));
	ajouterBouton(new Bouton(200, 85, "-"));
	ajouterLabel(new Label(145, 85, "5"));
	ajouterBouton(new Bouton(90, 85, "+"));
	ajouterBouton(new Bouton(200, 120, "Generate"));
	ajouterBouton(new Bouton(200, 155, "Render Mode"));
	ajouterBouton(new Bouton(200, 190, "Export OBJ"));*/

	//Configuration bandeau en haut
	ajouterLabel(new Label(820, 40, "Map Size"));
	ajouterBouton(new Bouton(675, 40, "-"));
	ajouterLabel(new Label(620, 40, "5"));
	ajouterBouton(new Bouton(565, 40, "+"));
	ajouterBouton(new Bouton(510, 40, "Generate"));
	ajouterBouton(new Bouton(375, 40, "Render Mode"));
	ajouterBouton(new Bouton(200, 40, "Export OBJ"));
	
	
	
	
	

}


InterfaceUtilisateur::~InterfaceUtilisateur()
{
}

bool InterfaceUtilisateur::getSurvol(){ return survolBouton; }
Label* InterfaceUtilisateur::getLabel(int i){ return labels[i]; }

void InterfaceUtilisateur::ajouterBouton(Bouton* newBut)
{
	boutons.push_back(newBut);
}
void InterfaceUtilisateur::ajouterLabel(Label* newLab)
{
	labels.push_back(newLab);
}

void InterfaceUtilisateur::testSurvol(int x, int y,  int windowW, int windowH)
{
	int index = 0;
	survolBouton = false;

	//Detection si l'un des boutons est survolé
	while (survolBouton == false && index < boutons.size())
	{
		survolBouton = boutons[index]->isMouseInMe(x, y, windowW, windowH);
		index++;
	}

	//Indice du bouton survolé
	if (survolBouton){ indexBut = index - 1; }
}

void InterfaceUtilisateur::draw(int windowW, int windowH)
{
	//Affiche l'intergace utilisateur
	//Les boutons
	for (int i = 0; i < boutons.size();i++)
	{
		boutons[i]->draw(windowW,windowH);
	}

	//Les labels
	for (int i = 0; i < labels.size(); i++)
	{
		labels[i]->draw(windowW, windowH);
	}
}

void InterfaceUtilisateur::action()
{
	//On choisi l'action en fonction du bouton ciblé
	string taille;
	int Taille;

	switch (indexBut)
	{
		//Bouton "-" on décrémente le label taille
		case 0:
			//On récupère le contenu du label Taille et on le converti en entier
			taille = labels[1]->getLabel();
			Taille = stoi(taille, nullptr);
			//Si le contenu est valide, on le décrémente.
			if (Taille > 1)
			{ 
				Taille--; 
				taille = to_string(Taille);
				labels[1]->setLabel(taille);
			}
		break;
		//Bouton "+" on incrément le label taille
		case 1:
			//On récupère le contenu du label Taille et on le converti en entier
			taille = labels[1]->getLabel();
			Taille = stoi(taille, nullptr);
			//Si le contenu est valide, on le décrémente.
			if (Taille <11)
			{
				Taille++;
				taille = to_string(Taille);
				labels[1]->setLabel(taille);
			}
		break;
		//Bouton "Generate" 
		case 2:
			//On change la taille de la map
			taille = labels[1]->getLabel();
			Taille = stoi(taille, nullptr);
			maMap->setLength(Taille);
			maMap->setTaille();

			//On détruit les buffers existants
			monVBO->DestroyVBO();

			//Réinitialisation caméra
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
			maMap->FillDataBuffersPosColors();

			// Remplissage du VBO
			monVBO->FeedCol(maMap->getCol());
			monVBO->FeedPos(maMap->getPos());
			monVBO->FeedTex(maMap->getTex());
			monVBO->BuildBuffer();

			break;
		case 3:
			//Changement de mode et retour au mode 0 après le mode 3
			renderMode++;
			if (renderMode == 4){ renderMode = 0; }

			//Choix du mode
				switch (renderMode)
			{
				case 0:
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					break;
					// Affichage en mode fil de fer
				case 1:
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					break;
					// Affichage en nuage de points
				case 2:
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					break;
				case 3:
					//TODO: Activer Shaders
					break;

				default:
					break;
			}
			break;

		case 4:
			maMap->ecrireFichierObj();
			break;

		default:
			break;

	}

}