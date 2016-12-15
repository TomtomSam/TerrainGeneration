#ifndef INTERFACEUTILISATEUR_H
#define INTERFACEUTILISATEUR_H

#include "heightMap.h"
#include "VBO.h"
#include "Bouton.h"
#include "Label.h"
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
	vector<Bouton*> boutons;
	vector<Label*> labels;

	//Est ce qu'il y a suvol d'un bouton
	bool survolBouton;
	//Quel est l'indice du bouton survolé
	int indexBut;
	//Mode de rendu
	int renderMode;

public:
	InterfaceUtilisateur(heightMap* _maMap, VBO* _monVBO, FreeFlyCamera* _maCamera);
	~InterfaceUtilisateur();

	bool getSurvol();
	Label* getLabel(int i);

	void ajouterBouton(Bouton* newBut);
	void ajouterLabel(Label* newLab);

	void testSurvol(int x, int y, int windowW, int windowH);

	void draw(int windowW, int windowH);
	
	void reductionTaille();
	void augmentationTaille();
	void generation();
	void renderModeSelection();
	void action();
};

#endif // INTERFACEUTILISATEUR_H