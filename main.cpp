#include <iostream>
#include <cstdlib>
#include <ctime>
#include "heightMap.h"

using namespace std;

int main()
{
    //Pour la génération aléatoire
    srand(time(NULL));

    //Génération manuelle d'une map
    heightMap maMap;
    maMap.initialisation();
    maMap.affichage();

    //Génération auto d'une map
    heightMap maMap1(2,2,-5,5);
    maMap1.initialisationAuto();
    maMap1.affichage();

    //Retour maison
    return 0;
}
