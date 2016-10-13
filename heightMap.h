#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>
using namespace std;

class heightMap
{
    private:
        int length;
        int width;
        int maxDepth;
        int maxHeight;
        vector< vector<int> > heightMatrix;

    public:
        //Getters
        int getLength();
        int getWidthth();
        int getMaxDepth();
        int getMaxHeight();
        int getHeightMap(int lig, int col);

        //Setters
        void setLength(int myLength);
        void setWidth(int myWidth);
        void setMaxDepth(int myMaxDepth);
        void setMaxHeight(int myMaxHeight);
        void setHeightMap(int lig, int col, int Height);

        //Methods
        void affichage();
        void initialisation();
        void initialisationAuto();
        void diamondStep();
        void squareStep();

        //Constructors
        heightMap();
        heightMap(int myLength, int myWidth, int myMaxDepth, int myMaxHeight);

        //Destructor
        virtual ~heightMap();
};

#endif // HEIGHTMAP_H
