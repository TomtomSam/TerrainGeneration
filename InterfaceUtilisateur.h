#ifndef INTERFACEUTILISATEUR_H
#define INTERFACEUTILISATEUR_H

#include "heightMap.h"
#include "VBO.h"
#include "IUobject.h"
#include "Bouton.h"
#include "Label.h"
#include "Curseur.h"
#include "FreeFlyCamera.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class InterfaceUtilisateur
{

private:
	heightMap* maMap;
	VBO* monVBO;
	FreeFlyCamera* maCamera;

	
	vector<IUobject*> objetsIU;	//Liste des objets dans l'interface utilisateur
	bool survolObject;			//Est ce qu'il y a suvol d'un object
	bool isClicked;				//Est ce qu'un objet est actif
	int indexObject;			//Quel est l'indice de l'objet survolé
	int renderMode;				//Mode de rendu


public:
	InterfaceUtilisateur(heightMap* _maMap, VBO* _monVBO, FreeFlyCamera* _maCamera);
	~InterfaceUtilisateur();

	//Getters et Setters
	bool getSurvol();
	bool getIsClicked();
	void setIsClicked(bool _isClicked);
	string getLabel(int i);
	int getRenderMode();
	IUobject* getObjet(int i);
	int getIndexObject();

	//Fonctions de l'interface
	void ajouterObjet(IUobject* newObj);
	void testSurvol(int x, int y, int windowW, int windowH);
	void draw(int windowW, int windowH);
	
	//Fonctions d'actions
	void action();
	void reductionTaille();
	void augmentationTaille();
	void generation();
	void renderModeSelection();
	void modifyOcean();
	void modifyDilatation();
	
	
};

#endif // INTERFACEUTILISATEUR_H