#include "heightMap.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
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
    maxIndexColumn=static_cast<int>(pow(2,length));
    maxIndexLine=static_cast<int>(pow(2,width));

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

	maxIndexColumn = static_cast<int>(pow(2, length));
	maxIndexLine = maxIndexColumn;

    //Corner's random height between maxDepth and maxHeight;
	/*corner1 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);
	corner2 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);
	corner3 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);
	corner4 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);*/
	
	corner1 = 0;
	corner2 = 0;
	corner3 = 0;
	corner4 = 0;

    //Setting the height of the map's corners
    setHeightMap(0,0,corner1);
    setHeightMap(0,maxIndexColumn,corner2);
    setHeightMap(maxIndexLine,maxIndexColumn,corner3);
    setHeightMap(maxIndexLine,0,corner4);
}


void heightMap::diamondStep(int pas){
    const int h=static_cast<int>(pow(2,length));
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

            random=static_cast<float>((rand()%(2*pas+1))-pas);
			while (abs(moyenne+random)>maxHeight)
			{
				random = static_cast<float>((rand() % (2 * pas + 1)) - pas);
			}
            heightMatrix[x][y]=moyenne+random;
            moyenne=0.0;
        }
    }
}

void heightMap::squareStep(int pas){
    const int h=static_cast<int>(pow(2,length));
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

            random=static_cast<float>((rand() % (2*pas+1))-pas);
			while (abs((somme / n) + random)>maxHeight)
			{
				random = static_cast<float>((rand() % (2 * pas + 1)) - pas);
			}
            heightMatrix[x][y]=(somme/n)+random;
        }
    }


}

float heightMap::getTaille(){
	return static_cast<float>(pow(2, getLength()));
}

void heightMap::generateMatrix(){
    int i=static_cast<int>(pow(2,length));
    int pas ;

    while(i>1)
    {
        pas=i/2;
        this->diamondStep(pas);
        this->squareStep(pas);
        i=pas;
    }
}

void heightMap::giveMaxes(float* max_min){
	float mini = maxHeight;
	float maxi = maxDepth;

	for (int a = 0; a <= pow(2, length); a++)
	{
		for (int b = 0; b <= pow(2, width); b++)
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

	max_min[0]=maxi;
	max_min[1]=mini;
}

void heightMap::vertexColor(float altitude, float snow, float beach, float water, float* color){
	float echelle_map=maxHeight-maxDepth;
	float altitude_ajuste=altitude-maxDepth;

	if(altitude>snow){
		color[0] = 1;
		color[1] = 1;
		color[2] = 1;
	}
	else if(altitude>beach){
		color[0] = 0;
		color[1] = altitude_ajuste/echelle_map;
		color[2] = 0;
	}
	else if(altitude>water){
		color[0] = altitude_ajuste/echelle_map;
		color[1] = altitude_ajuste/echelle_map;
		color[2] = 0;
	}
}

void heightMap::seuilDefinition(float* seuil){
	float maximini[2];
	giveMaxes(maximini);
	float distance_minmax=maximini[0]-maximini[1];

	seuil[0] = static_cast<float>(0.8*distance_minmax	+	maximini[1]);//snow
	seuil[1] = static_cast<float>(0.35*distance_minmax	+	maximini[1]);//beach
	seuil[2]=static_cast<float>(0.3*distance_minmax		+	maximini[1]);//water
}

void heightMap::ecrireFichierObj(){

	int taille = pow(2, getLength());
	int nbrPoint = pow(taille + 1, 2);
	ofstream myfile;
	myfile.open("map.obj",ios::out);
	myfile << "#map.obj\n";
	myfile << "#\n\n";
	myfile << "o Map\n\n";

	//Ecriture des vertex
	for (int i = 0; i <= taille; i++)
	{
		for (int j = 0; j <= taille; j++)
		{
			myfile << "v "<<i<<" "<<getHeightMap(i,j)<<" "<<j<<"\n";
		}
	}

	myfile << "\n";

	//ecriture des facettes
	for (int j = 1; j < nbrPoint-taille-1; j++)
	{
		if (j % (taille+1) != 0)
		{
			myfile << "f " << j << " " << j + 1 << " " << j + taille + 1 << "\n";
		}

		
	}

	for (int j = 2; j < nbrPoint - taille; j++)
	{
		if (j % (taille + 1) != 1)
		{
			myfile << "f " << j  << " " << j + taille + 1 << " " << j + taille  << "\n";
		}
	}
	


	myfile.close();
	
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

	if (taille > 15){ taille = 15; }

	setLength(taille);
	setWidth(taille);
	setMaxDepth(static_cast<float>(-pow(2, taille)));
	setMaxHeight(static_cast<float>(pow(2,taille)));

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
