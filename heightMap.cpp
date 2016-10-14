#include "heightMap.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
using namespace std;

//GETTERS
int heightMap::getLength(){
    return length;
}

int heightMap::getWidth(){
    return width;
}

float heightMap::getMaxDepth(){
    return maxDepth;
}

float heightMap::getMaxHeight(){
    return maxHeight;
}

float heightMap::getHeightMap(int lig, int col){
    return heightMatrix[lig][col];
}

//SETTERS
void heightMap::setLength(int myLength){
    length=myLength;
}

void heightMap::setWidth(int myWidth){
    width=myWidth;
}

void heightMap::setMaxDepth(float myMaxDepth){
    maxDepth=myMaxDepth;
}

void heightMap::setMaxHeight(float myMaxHeight){
    maxHeight=myMaxHeight;
}

void heightMap::setHeightMap(int lig, int col, float Height){
    heightMatrix[lig][col]=Height;
}

//METHODS
void heightMap::initialisation(){
    float corner1,corner2,corner3,corner4; //Height of the corners
    int maxIndexColumn,maxIndexLine;//Last column/line of the map

    cout<<"Parameters of the Map:"<<endl;
    cout<<"Length: ";//Choice of the map's length
    cin>>length;
    cout<<"Width: ";//Choice of the map's width
    cin>>width;
    cout<<"Maximum depth: ";//Choice of the map's minimum height
    cin>>maxDepth;
    cout<<"Maximum height: ";//Choice of the map's maximum height
    cin>>maxHeight;

    do{//Choice of the top left corner's height
        cout<<"Altitude of top left corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
        cin>>corner1;
    }
    while((corner1>maxHeight)||(corner1<maxDepth));

    do{//Choice of the top right corner's height
        cout<<"Altitude of top right corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
        cin>>corner2;
    }
    while((corner2>maxHeight)||(corner2<maxDepth));

    do{//Choice of the bottom right corner's height
        cout<<"Altitude of bottom right corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
        cin>>corner3;
    }
    while((corner3>maxHeight)||(corner3<maxDepth));

    do{//Choice of the bottom left corner's height
        cout<<"Altitude of bottom left corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
        cin>>corner4;
    }
    while((corner4>maxHeight)||(corner4<maxDepth));

    cout<<endl;

    //Setting maxIndexColumn and maxIndexLine
    maxIndexColumn=pow(2,length);
    maxIndexLine=pow(2,width);

    //Setting the height of the map's corners
    setHeightMap(0,0,corner1);
    setHeightMap(0,maxIndexColumn,corner2);
    setHeightMap(maxIndexLine,maxIndexColumn,corner3);
    setHeightMap(maxIndexLine,0,corner4);
}

void heightMap::initialisationAuto(){
    float corner1,corner2,corner3,corner4; //Height of the corners
    int maxIndexColumn,maxIndexLine;//Last column/line of the map
	int signe = 0;

	maxIndexColumn = pow(2, length);
	maxIndexLine = maxIndexColumn;

    //Corner's random height between maxDepth and maxHeight;
	corner1 = (rand() % (2 * maxIndexLine + 1)) - maxIndexLine;
	corner2 = (rand() % (2 * maxIndexLine + 1)) - maxIndexLine;
	corner3 = (rand() % (2 * maxIndexLine + 1)) - maxIndexLine;
	corner4 = (rand() % (2 * maxIndexLine + 1)) - maxIndexLine;

    //Setting the height of the map's corners
    setHeightMap(0,0,corner1);
    setHeightMap(0,maxIndexColumn,corner2);
    setHeightMap(maxIndexLine,maxIndexColumn,corner3);
    setHeightMap(maxIndexLine,0,corner4);
}

void heightMap::affichageTest(){ //Pour les test avec la console
    for(int l=0;l<1+pow(2,width);l++)
    {
        for(int c=0;c<1+pow(2,length);c++)
        {
            cout<<round(heightMatrix[l][c])<<"\t"; //on arrondi l'afficher du float à l'entier le plus proche pour avoir un affichage plus propre
        }
        cout<<endl;
    }
    cout<<endl;
}

void heightMap::diamondStep(int pas){
    const int h=pow(2,length);
    float moyenne=0.0;
    float random=0.0;
    int i=2*pas;

    for(int x=pas;x<=h;x+=i)
    {
        for(int y=pas;y<=h;y+=i)
        {
            moyenne+=heightMatrix[x-pas][y-pas];
            moyenne+=heightMatrix[x-pas][y+pas];
            moyenne+=heightMatrix[x+pas][y+pas];
            moyenne+=heightMatrix[x+pas][y-pas];
            moyenne/=4;

            random=(rand()%(2*pas+1))-pas;
            heightMatrix[x][y]=moyenne+random;
            moyenne=0.0;
        }
    }
}

void heightMap::squareStep(int pas){
    const int h=pow(2,length);
    float somme=0.0;
    int n=0;
    int decalage=0;
    float random=0.0;
    int i=2*pas;

    for(int x=0;x<=h;x+=pas)
    {
        if(x % i == 0)
        {
            decalage=pas;
        }
        else
        {
            decalage=0;
        }

        for(int y=decalage;y<=h;y+=i)
        {
            somme=0.0;
            n=0;
            if(x>=pas)
            {
                somme+=heightMatrix[x-pas][y];
                n++;
            }
            if(x+pas<h)
            {
                somme+=heightMatrix[x+pas][y];
                n++;
            }
            if(y>=pas)
            {
                somme+=heightMatrix[x][y-pas];
                n++;
            }
            if(y+pas<h)
            {
                somme+=heightMatrix[x][y+pas];
                n++;
            }

            random=(rand() % (2*pas+1))-pas;
            heightMatrix[x][y]=(somme/n)+random;
        }
    }


}

void heightMap::generateMatrix(){
    int i=pow(2,length);
    int pas ;

    while(i>1)
    {
        pas=i/2;
        this->diamondStep(pas);
        this->squareStep(pas);
        i=pas;
    }

    this->rescale();
}

void heightMap::rescale(){
	int* maxMin = giveMaxes();
    float maxi=maxMin[0];
	float mini = maxMin[1];
    float rescaleFactor=0.0;
    bool rescale_needed=false;

    if(maxi>maxHeight || mini<maxDepth)
    {
        rescaleFactor = (maxHeight-maxDepth)/(maxi-mini);

        for(int a=0; a<=pow(2,length); a++)
        {
            for(int b=0; b<=pow(2,width); b++)
            {
                heightMatrix[a][b]=(heightMatrix[a][b]-mini)*rescaleFactor;
            }
        }

        for(int a=0; a<=pow(2,length); a++)
        {
            for(int b=0; b<=pow(2,width); b++)
            {
                heightMatrix[a][b]+=maxDepth;
            }
        }
    }
}

int* heightMap::giveMaxes(){
	float mini = this->maxHeight;
	float maxi = this->maxDepth;
	float rescaleFactor = 0.0;

	for (int a = 0; a <= pow(2, this->length); a++)
	{
		for (int b = 0; b <= pow(2, this->width); b++)
		{
			if (heightMatrix[a][b]>maxi)
			{
				maxi = heightMatrix[a][b];
			}
			if (heightMatrix[a][b]<mini)
			{
				mini = heightMatrix[a][b];
			}
		}
	}

	int maxMin[2] = { maxi, mini };

	return maxMin;
}

//CONSTRUCTORS
heightMap::heightMap(){
     setLength(0);
     setWidth(0);
     setMaxDepth(0.0);
     setMaxHeight(0.0);

    for(int l=0;l<1024;l++) //By default the map's dimensions are 1024x1024
    {
        vector<float> row;
        for(int c=0;c<1024;c++)
        {
            row.push_back(0.0);
        }
        heightMatrix.push_back(row);
    }

}

heightMap::heightMap(int taille){
	setLength(taille);
	setWidth(taille);
	setMaxDepth(0);
	setMaxHeight(pow(2,taille));

	for (int l = 0; l<1 + pow(2, taille); l++)
	{
		vector<float> row;
		for (int c = 0; c<1 + pow(2, taille); c++)
		{
			row.push_back(0.0);
		}
		heightMatrix.push_back(row);
	}
}

heightMap::heightMap(int myLength, int myWidth, float myMaxDepth, float myMaxHeight){
     setLength(myLength);
     setWidth(myWidth);
     setMaxDepth(myMaxDepth);
     setMaxHeight(myMaxHeight);

    for(int l=0;l<1+pow(2,myWidth);l++)
    {
        vector<float> row;
        for(int c=0;c<1+pow(2,myLength);c++)
        {
            row.push_back(0.0);
        }
        heightMatrix.push_back(row);
    }
}

//DESTRUCTOR
heightMap::~heightMap(){
    //dtor
}
