#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Point.h"
#include "ColorRamp.h"
#include "Chrono.h"
using namespace std;

class heightMap
{
    private:
        int length;
        int width;
		int taille;
        float maxDepth;
        float maxHeight;
		float posOcean;
		float dilatation; 
        vector< vector<Point*> > heightMatrix;
		GLenum renderMode;

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
		float getDilatation();
        Point* getHeightMap(int lig, int col);
		vector<float> getPos();
		vector<float> getCol();
		vector<float> getTex();
		int getTaille();
		GLenum  getRenderMode();

        //Setters
        void setLength(int myLength);
        void setWidth(int myWidth);
        void setMaxDepth(float myMaxDepth);
        void setMaxHeight(float myMaxHeight);
        void setHeightMap(int lig, int col, Point* point);
		void setPosOcean(float _pos);
		void setDilatation(float _dilatation);
		void setTaille();
		void setMatrix(vector<Point*> newRow);
		void clearMatrix();
		void resetDilatation();
		void setRenderMode(GLenum _renderMode);

        //Methods
        void initialisation();
        void initialisationAuto();
        void generateMatrix();
        void diamondStep( int pas);
        void squareStep(int pas);
        void giveMaxes(float* max_min);
		void mapColor();
		void ecrireFichierObj();
		void FillDataBuffersPosColorsTex();
		void FillDataBuffersColors();
		void FillDataBuffersPos();
		void FillDataBuffersTex();
		void FillDataPosCacheMisere();
		void compteurFPS(int windowW, int windowH, int FPS);
		void dessinOcean();
		void dessinOceanPreview(float posOceanPreview);
		void dessinDilatationPreview(float dilatationPreview);

        //Constructors
        heightMap();
        heightMap(int taille);
        heightMap(int myLength, int myWidth, float myMaxDepth, float myMaxHeight);

        //Destructor
        virtual ~heightMap();
};

#endif // HEIGHTMAP_H