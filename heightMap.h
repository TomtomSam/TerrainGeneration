#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include "Point.h"
#include "ColorRamp.h"
#include "Chrono.h"
using namespace std;

class heightMap
{
    private:
        int length;
        int width;
        float maxDepth;
        float maxHeight;
		float posOcean;
        vector< vector<Point*> > heightMatrix;

		//Data pour le VBO
		vector<float> pos;
		vector<float> colors;
		vector<float> tex;

		//Rampe de couleur Biomes
		ColorRamp laRampe;

    public:
        //Getters
        int getLength();
        int getWidth();
        float getMaxDepth();
        float getMaxHeight();
		float getPosOcean();
        Point* getHeightMap(int lig, int col);
		vector<float> getPos();
		vector<float> getCol();
		vector<float> getTex();

        //Setters
        void setLength(int myLength);
        void setWidth(int myWidth);
        void setMaxDepth(float myMaxDepth);
        void setMaxHeight(float myMaxHeight);
        void setHeightMap(int lig, int col, Point* point);
		void setPosOcean(float _pos);

        //Methods
        void initialisation();
        void initialisationAuto();
        void generateMatrix();
        void diamondStep( int pas);
        void squareStep(int pas);
		float getTaille();
        void giveMaxes(float* max_min);
		void mapColor();
		void seuilDefinition(float* seuil);
		void ecrireFichierObj();
		void FillDataBuffersPosColors();

        //Constructors
        heightMap();
        heightMap(int taille);
        heightMap(int myLength, int myWidth, float myMaxDepth, float myMaxHeight);

        //Destructor
        virtual ~heightMap();
};

#endif // HEIGHTMAP_H