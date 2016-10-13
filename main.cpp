#include <iostream>
#include <cstdlib>
#include <ctime>
#include "heightMap.h"

using namespace std;

int main()
{
    //Pour la g�n�ration al�atoire
    srand(time(NULL));

    //G�n�ration manuelle d'une map
    heightMap maMap;
    maMap.initialisation();
    maMap.affichage();

    //G�n�ration auto d'une map
    heightMap maMap1(2,2,-5,5);
    maMap1.initialisationAuto();
    maMap1.affichage();

    //Retour maison
    return 0;
}
