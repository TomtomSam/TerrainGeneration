#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>
#include "Seuil.h"
using namespace std;

class heightMap
{
    private:
        int length;
        int width;
        float maxDepth;
        float maxHeight;
        vector< vector<float> > heightMatrix;

    public:
        //Getters
        int getLength();
        int getWidth();
        float getMaxDepth();
        float getMaxHeight();
        float getHeightMap(int lig, int col);

        //Setters
        void setLength(int myLength);
        void setWidth(int myWidth);
        void setMaxDepth(float myMaxDepth);
        void setMaxHeight(float myMaxHeight);
        void setHeightMap(int lig, int col, float Height);

        //Methods
        void initialisation();
        void initialisationAuto();
        void generateMatrix();
        void diamondStep( int pas);
        void squareStep(int pas);
		float getTaille();
        void giveMaxes(float* max_min);
		void vertexColor(float altitude, Seuil snow, Seuil beach, Seuil water, float* color);
		Seuil seuilDefinition(Seuil limite, float reglage);
		void ecrireFichierObj();

        //Constructors
        heightMap();
        heightMap(int taille);
        heightMap(int myLength, int myWidth, float myMaxDepth, float myMaxHeight);

        //Destructor
        virtual ~heightMap();
};

#endif // HEIGHTMAP_H
