#include "ColorRamp.h"

/*
La rampe de couleur est créée après la génération de la matrice
des hauteurs et prend en entrée la différence entre la hauteur max et min
de la map afin de faire correspondre une couleur à une altitude.
*/



void ColorRamp::Remplissage(float HauteurMax, float posOcean)
{//Limites RGB des biomes
//10% 45% 70% 90%
//Plage Jaune
	float PLAGE_R =228;
	float PLAGE_G =178;
	float PLAGE_B =36;
	//PLAINE vert clair
	float PLAINE_R =146;
	float PLAINE_G =208;
	float PLAINE_B =80;
	//HAUTE PLAINE vert foncé
	float HPLAINE_R =24;
	float HPLAINE_G =122;
	float HPLAINE_B =29;
	//BASSE MONTAGNE marron
	float BMONT_R =66;
	float BMONT_G =39;
	float BMONT_B =7;
	//Rocaille gris foncé
	float ROCK =125;
	//NEIGE
	float NEIGE =255;

	//Nettoyage des vecteurs
	R.clear();
	G.clear();
	B.clear();

	//Difference de hauteur entre l'océan et le plus haut point
	int Envergure = static_cast<int>(HauteurMax-posOcean)+1;

	//Envergure de chaque phase
	int plage_plaine = static_cast<int>(0.1*Envergure);
	int plaine_Hplaine = static_cast<int>(0.1*Envergure);
	int Hplaine_Mont = static_cast<int>(0.5*Envergure);
	int Mont_Rock = static_cast<int>(0.1*Envergure);
	int Rock_Neige = static_cast<int>(0.2*Envergure);
	
	R += Lerp(PLAGE_R / 255, PLAINE_R / 255, plage_plaine);
	R += Lerp(PLAINE_R / 255, HPLAINE_R / 255, plaine_Hplaine);
	R += Lerp(HPLAINE_R / 255, BMONT_R / 255, Hplaine_Mont);
	R += Lerp(BMONT_R / 255, ROCK / 255, Mont_Rock);
	R += Lerp(ROCK / 255, NEIGE / 255, Rock_Neige);

	G += Lerp(PLAGE_G / 255, PLAINE_G / 255, plage_plaine);
	G += Lerp(PLAINE_G / 255, HPLAINE_G / 255, plaine_Hplaine);
	G += Lerp(HPLAINE_G / 255, BMONT_G / 255, Hplaine_Mont);
	G += Lerp(BMONT_G / 255, ROCK / 255, Mont_Rock);
	G += Lerp(ROCK / 255, NEIGE / 255, Rock_Neige);

	B += Lerp(PLAGE_B / 255, PLAINE_B / 255, plage_plaine);
	B += Lerp(PLAINE_B / 255, HPLAINE_B / 255, plaine_Hplaine);
	B += Lerp(HPLAINE_B / 255, BMONT_B / 255, Hplaine_Mont);
	B += Lerp(BMONT_B / 255, ROCK / 255, Mont_Rock);
	B += Lerp(ROCK / 255, NEIGE / 255, Rock_Neige);
	

	//On rempli avec de la neige
	do{
		R.push_back(1);
		G.push_back(1);
		B.push_back(1);
	} while (static_cast<int>(R.size()) <= Envergure);
	
	
}

vector<float> ColorRamp::Lerp(float col1, float col2, int env)
{
	vector<float> tab;
	tab.clear();

	float increment = (col2 - col1) / env;

	for (int i = 0; i < env; i++)
	{
		tab.push_back(col1 + i*increment);
	}
	return tab;
}

float ColorRamp::getR(int i){ return R[i]; }
float ColorRamp::getG(int i){ return G[i]; }
float ColorRamp::getB(int i){ return B[i]; }

ColorRamp::ColorRamp()
{
}


ColorRamp::~ColorRamp()
{
}

vector<float>& operator+=(vector<float>& v1, vector<float>& v2){

	for (int i = 0; i<static_cast<int>(v2.size()); i++)
	{
		v1.push_back(v2[i]);
	}

	return v1;

}
