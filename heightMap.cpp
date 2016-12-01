#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include "Chrono.h"
#include "heightMap.h"
using namespace std;

//GETTERS
int heightMap::getLength(){return length;}
int heightMap::getWidth(){return width;}
float heightMap::getMaxDepth(){return maxDepth;}
float heightMap::getMaxHeight(){return maxHeight;}
Point* heightMap::getHeightMap(int lig, int col){return heightMatrix[lig][col];}
vector<float> heightMap::getPos(){ return pos; }
vector<float> heightMap::getCol(){ return colors; }
vector<float> heightMap::getTex(){ return tex; }
float heightMap::getPosOcean(){ return posOcean; }
float heightMap::getDilatation(){ return dilatation; }
float heightMap::getTaille(){return static_cast<float>(pow(2, getLength()));}

//SETTERS
void heightMap::setLength(int myLength){length=myLength;}
void heightMap::setWidth(int myWidth){width=myWidth;}
void heightMap::setMaxDepth(float myMaxDepth){maxDepth=myMaxDepth;}
void heightMap::setMaxHeight(float myMaxHeight){maxHeight=myMaxHeight;}
void heightMap::setHeightMap(int lig, int col, Point* point){heightMatrix[lig][col]=point;}

void heightMap::setPosOcean(float _pos)
{	posOcean = _pos;
	FillDataBuffersPosColors();
}

void heightMap::setDilatation(float _dilatation)
{
	if(_dilatation<1){dilatation = 1;}
	else{dilatation = _dilatation;}
	FillDataBuffersPosColors();
}

//METHODS
void heightMap::initialisation(){
	float corner1,corner2,corner3,corner4; // Height of the corners
	int maxIndexColumn,maxIndexLine;// Last column/line of the map

	cout<<"Parameters of the Map:"<<endl;
	cout<<"Length: ";// Choice of the map's length
	cin>>length;
	cout<<"Width: ";// Choice of the map's width
	cin>>width;
	cout<<"Maximum depth: ";// Choice of the map's minimum height
	cin>>maxDepth;
	cout<<"Maximum height: ";// Choice of the map's maximum height
	cin>>maxHeight;

	do{// Choice of the top left corner's height
		cout<<"Altitude of top left corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner1;
	}
	while((corner1>maxHeight)||(corner1<maxDepth));

	do{// Choice of the top right corner's height
		cout<<"Altitude of top right corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner2;
	}
	while((corner2>maxHeight)||(corner2<maxDepth));

	do{// Choice of the bottom right corner's height
		cout<<"Altitude of bottom right corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner3;
	}
	while((corner3>maxHeight)||(corner3<maxDepth));

	do{// Choice of the bottom left corner's height
		cout<<"Altitude of bottom left corner (must be between "<<maxDepth<<" and "<<maxHeight<<") : ";
		cin>>corner4;
	}
	while((corner4>maxHeight)||(corner4<maxDepth));

	cout<<endl;

	// Setting maxIndexColumn and maxIndexLine
	maxIndexColumn=static_cast<int>(pow(2,length));
	maxIndexLine=static_cast<int>(pow(2,width));

	// Setting the height of the map's corners
	setHeightMap(0,0,new Point(0,0,corner1));
	setHeightMap(0, maxIndexColumn, new Point(0, maxIndexColumn, corner2));
	setHeightMap(maxIndexLine, maxIndexColumn, new Point(maxIndexLine, maxIndexColumn, corner3));
	setHeightMap(maxIndexLine, 0, new Point(maxIndexLine, 0, corner4));
}

void heightMap::initialisationAuto(){
	float corner1,corner2,corner3,corner4; //Height of the corners
	int maxIndexColumn,maxIndexLine;//Last column/line of the map
	int signe = 0;

	maxIndexColumn = static_cast<int>(pow(2, length));
	maxIndexLine = maxIndexColumn;

	// Corner's random height between maxDepth and maxHeight
	/*corner1 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);
	corner2 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);
	corner3 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);
	corner4 = static_cast<float>((rand() % (2 * maxIndexLine + 1)) - maxIndexLine);*/

	// Corner's height set at 0
	corner1 = 0;
	corner2 = 0;
	corner3 = 0;
	corner4 = 0;

	// Setting the height of the map's corners
	setHeightMap(0, 0, new Point(0, 0, corner1));
	setHeightMap(0, maxIndexColumn, new Point(0, maxIndexColumn, corner2));
	setHeightMap(maxIndexLine, maxIndexColumn, new Point(maxIndexLine, maxIndexColumn, corner3));
	setHeightMap(maxIndexLine, 0, new Point(maxIndexLine, 0, corner4));
}


void heightMap::diamondStep(int pas){
	const int h=static_cast<int>(1+pow(2,length));
	float moyenne=0.0;
	float random=0.0;
	int i=2*pas;

	for(int x=pas; x<h; x+=i)
	{
		for(int y=pas; y<h; y+=i)
		{
			moyenne += heightMatrix[x-pas][y-pas]->getHeight();
			moyenne += heightMatrix[x-pas][y+pas]->getHeight();
			moyenne += heightMatrix[x+pas][y+pas]->getHeight();
			moyenne += heightMatrix[x+pas][y-pas]->getHeight();
			moyenne/=4;

			random=static_cast<float>((rand()%(2*pas+1))-pas);
			while (abs(moyenne+random)>maxHeight)
			{
				random = static_cast<float>((rand()%(2*pas+1))-pas);
			}
			heightMatrix[x][y]->setHeight(moyenne+random);
			moyenne=0.0;
		}
	}
}

void heightMap::squareStep(int pas){
	const int h=static_cast<int>(1+pow(2,length));
	float somme=0.0;
	int n=0;
	int decalage=0;
	float random=0.0;
	int i=2*pas;

	for(int x=0; x<h; x+=pas)
	{
		if(x % i == 0){decalage=pas;}
		else{decalage=0;}

		for(int y=decalage; y<h; y+=i)
		{
			somme=0.0;
			n=0;
			if(x>=pas)
			{
				somme += heightMatrix[x-pas][y]->getHeight();
				n++;
			}
			if(x+pas<h)
			{
				somme += heightMatrix[x+pas][y]->getHeight();
				n++;
			}
			if(y>=pas)
			{
				somme += heightMatrix[x][y-pas]->getHeight();
				n++;
			}
			if(y+pas<h)
			{
				somme += heightMatrix[x][y+pas]->getHeight();
				n++;
			}

			random=static_cast<float>((rand()%(2*pas+1))-pas);
			while (abs((somme/n)+random)>maxHeight)
			{
				random = static_cast<float>((rand()%(2*pas +1))-pas);
			}
			heightMatrix[x][y]->setHeight((somme/n)+random);
		}
	}
}

void heightMap::generateMatrix(){
	Chrono chrono;
	chrono.Tic();

	int i=static_cast<int>(pow(2,length));
	int pas;

	while(i>1)
	{
		pas=i/2;
		this->diamondStep(pas);
		this->squareStep(pas);
		i=pas;
	}

	float maxes[2];
	giveMaxes(maxes);
	posOcean = static_cast<float>((maxes[0] - maxes[1])*0.3 + maxes[1]);

	chrono.Toc();
	cout << "Generation effectuee en " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s." << endl;
}

void heightMap::giveMaxes(float* max_min){
	float mini = maxHeight;
	float maxi = maxDepth;

	for (int a = 0; a <= pow(2, length); a++)
	{
		for (int b = 0; b <= pow(2, width); b++)
		{
			if (heightMatrix[a][b]->getHeight()>maxi)
			{
				maxi = heightMatrix[a][b]->getHeight();
			}
			if (heightMatrix[a][b]->getHeight()<mini)
			{
				mini = heightMatrix[a][b]->getHeight();
			}
		}
	}

	max_min[0]=maxi;
	max_min[1]=mini;
}


void heightMap::mapColor()
{
	//On rempli la rampe de couleur quand on a toutes les hauteurs
	float maxes[2];
	giveMaxes(maxes);
	laRampe.Remplissage(maxes[0], posOcean);

	int indice = 0;

	for (int l = 0; l<1+pow(2,width); l++)
	{
		for (int c = 0; c<1+pow(2,length); c++)
		{	
			//Si on est sous l'océan on est couleur sable
			if (heightMatrix[l][c]->getHeight() < posOcean)
			{
				heightMatrix[l][c]->setR(laRampe.getR(0));
				heightMatrix[l][c]->setG(laRampe.getG(0));
				heightMatrix[l][c]->setB(laRampe.getB(0));
			}
			else
			{
				indice = (int)(heightMatrix[l][c]->getHeight() - posOcean);
				heightMatrix[l][c]->setR(laRampe.getR(indice));
				heightMatrix[l][c]->setG(laRampe.getG(indice));
				heightMatrix[l][c]->setB(laRampe.getB(indice));
			}	
		}
	}
}


void heightMap::ecrireFichierObj(){
	Chrono chrono;
	chrono.Tic();
	cout << "Ecriture du fichier OBJ, veuillez patienter..." << endl;

	int taille = static_cast<int>(pow(2, getLength()));
	int nbrPoint = static_cast<int>(pow(taille + 1, 2));
	ofstream myfile;
	myfile.open("map.obj",ios::out);
	myfile << "#map.obj\n";
	myfile << "#\n\n";
	myfile << "o Map\n\n";

	//Ecriture des vertex
	for (int i = 0; i < 1+taille; i++)
	{
		for (int j = 0; j < 1+taille; j++)
		{
			myfile << "v " << i << " " << getHeightMap(i, j)->getHeight() << " " << j << "\n";
		}
	}

	myfile << "\n";

	//ecriture des facettes
	for (int j = 1; j < nbrPoint-taille-1; j++)
	{
		if (j%(taille+1) != 0)
		{
			myfile << "f " << j << " " << j + 1 << " " << j + taille + 1 << "\n";
		}
	}

	for (int j = 2; j < nbrPoint - taille; j++)
	{
		if (j%(taille+1) != 1)
		{
			myfile << "f " << j  << " " << j + taille + 1 << " " << j + taille  << "\n";
		}
	}

	myfile.close();

	chrono.Toc();
	cout << "Fichier OBJ genere en " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s" << endl;

}

// Remplissage du VBO
void heightMap::FillDataBuffersPosColors()
{
	int taille = static_cast<int>(pow(2, length));
	float stride = 1.0f / taille;

	Chrono chrono;
	chrono.Tic();

	pos.clear();
	colors.clear();
	tex.clear();

	//On recalcule les couleurs de chaque point
	mapColor();

	// Remplissage strip par strip
	for (int i = 0; i < taille; i += 2)
	{

		// Remplissage strip vers la droite
		for (int j = 0; j < 1+taille; j++)
		{
			// Les Positions 
			/*pos.push_back(dilatation*static_cast<float>(i));
			pos.push_back(heightMatrix[i][j]->getHeight());
			pos.push_back(dilatation*static_cast<float>(j));
			pos.push_back(dilatation*static_cast<float>(i+1));
			pos.push_back(heightMatrix[i+1][j]->getHeight());
			pos.push_back(dilatation*static_cast<float>(j));*/
			pos.push_back(static_cast<float>(i));
			pos.push_back(heightMatrix[i][j]->getHeight());
			pos.push_back(static_cast<float>(j));
			pos.push_back(static_cast<float>(i+1));
			pos.push_back(heightMatrix[i+1][j]->getHeight());
			pos.push_back(static_cast<float>(j));

			// Les couleurs
			colors.push_back(heightMatrix[i][j]->getR());
			colors.push_back(heightMatrix[i][j]->getG());
			colors.push_back(heightMatrix[i][j]->getB());
			colors.push_back(heightMatrix[i+1][j]->getR());
			colors.push_back(heightMatrix[i+1][j]->getG());
			colors.push_back(heightMatrix[i+1][j]->getB());

			// Les textures
			tex.push_back(i*stride);
			tex.push_back(j*stride);
			tex.push_back((i+1)*stride);
			tex.push_back(j*stride);
		}

		// Le dernier point de chaque strip est rentré deux fois dans le vecteur pour faire le virage
		// Remplissage strip vers la gauche
		for (int k = taille; k > -1; k--)
		{
			// Les Positions
			/*pos.push_back(dilatation*static_cast<float>(i+1));
			pos.push_back(heightMatrix[i + 1][k]->getHeight());
			pos.push_back(dilatation*static_cast<float>(k));
			pos.push_back(dilatation*static_cast<float>(i+2));
			pos.push_back(heightMatrix[i + 2][k]->getHeight());
			pos.push_back(dilatation*static_cast<float>(k));*/
			pos.push_back(static_cast<float>(i+1));
			pos.push_back(heightMatrix[i + 1][k]->getHeight());
			pos.push_back(static_cast<float>(k));
			pos.push_back(static_cast<float>(i+2));
			pos.push_back(heightMatrix[i + 2][k]->getHeight());
			pos.push_back(static_cast<float>(k));

			//Les couleurs
			colors.push_back(heightMatrix[i+1][k]->getR());
			colors.push_back(heightMatrix[i+1][k]->getG());
			colors.push_back(heightMatrix[i+1][k]->getB());
			colors.push_back(heightMatrix[i+2][k]->getR());
			colors.push_back(heightMatrix[i+2][k]->getG());
			colors.push_back(heightMatrix[i+2][k]->getB());

			// Les textures
			tex.push_back((i+1)*stride);
			tex.push_back(k*stride);
			tex.push_back((i+2)*stride);
			tex.push_back(k*stride);
		}
	}

	chrono.Toc();
	cout << "Remplissage des donnees effectue en: " << static_cast<float>(chrono.getEllapsed_time()) / 1000 << "s." << endl;
}

//CONSTRUCTORS
heightMap::heightMap(){
	setLength(0);
	setWidth(0);
	setMaxDepth(0.0);
	setMaxHeight(0.0);
	setDilatation(1.0);


	for(int l=0; l<1024; l++) //By default the map's dimensions are 1024x1024
	{
		vector<Point*> row;
		for(int c=0; c<1024; c++)
		{
			row.push_back(new Point(0,0,0));
		}
		heightMatrix.push_back(row);
	}
}

heightMap::heightMap(int taille){

	// On limite la taille de la map a 2^12+1 pour pas consommer toute la memoire
	if (taille > 13){ taille = 12; }

	setDilatation(1.0);
	setLength(taille);
	setWidth(taille);
	setMaxDepth(static_cast<float>(-pow(2, taille)));
	setMaxHeight(static_cast<float>(pow(2,taille)));

	for (int l = 0; l<1 + pow(2, taille); l++)
	{
		vector<Point*> row;
		for (int c = 0; c<1 + pow(2, taille); c++)
		{
			row.push_back(new Point(0, 0, 0));
		}
		heightMatrix.push_back(row);
	}

	//Clear the data
	pos.clear();
	colors.clear();
	tex.clear();
}

heightMap::heightMap(int myLength, int myWidth, float myMaxDepth, float myMaxHeight){
	setDilatation(1.0); 
	setLength(myLength);
	setWidth(myWidth);
	setMaxDepth(myMaxDepth);
	setMaxHeight(myMaxHeight);

	for(int l=0;l<1+pow(2,myWidth);l++)
	{
		vector<Point *> row;
		for(int c=0;c<1+pow(2,myLength);c++)
		{
			row.push_back(new Point(0, 0, 0));
		}
		heightMatrix.push_back(row);
	}
}

//DESTRUCTOR
heightMap::~heightMap(){
	//dtor
}
