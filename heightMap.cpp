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

int heightMap::getWidthth(){
    return width;
}

int heightMap::getMaxDepth(){
    return maxDepth;
}

int heightMap::getMaxHeight(){
    return maxHeight;
}

int heightMap::getHeightMap(int lig, int col){
    return heightMatrix[lig][col];
}

//SETTERS
void heightMap::setLength(int myLength){
    length=myLength;
}

void heightMap::setWidth(int myWidth){
    width=myWidth;
}

void heightMap::setMaxDepth(int myMaxDepth){
    maxDepth=myMaxDepth;
}

void heightMap::setMaxHeight(int myMaxHeight){
    maxHeight=myMaxHeight;
}

void heightMap::setHeightMap(int lig, int col, int Height){
    heightMatrix[lig][col]=Height;
}

//METHODS
void heightMap::initialisation(){
    int corner1,corner2,corner3,corner4; //Height of the corners
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
    int corner1,corner2,corner3,corner4; //Height of the corners
    int maxIndexColumn,maxIndexLine;//Last column/line of the map

    //Corner's random height between maxDepth and maxHeight;
    corner1=maxDepth+(rand()%(maxHeight-maxDepth));
    corner2=maxDepth+(rand()%(maxHeight-maxDepth));
    corner3=maxDepth+(rand()%(maxHeight-maxDepth));
    corner4=maxDepth+(rand()%(maxHeight-maxDepth));

    //Setting maxIndexColumn and maxIndexLine
    maxIndexColumn=pow(2,length);
    maxIndexLine=pow(2,width);

    //Setting the height of the map's corners
    setHeightMap(0,0,corner1);
    setHeightMap(0,maxIndexColumn,corner2);
    setHeightMap(maxIndexLine,maxIndexColumn,corner3);
    setHeightMap(maxIndexLine,0,corner4);
}

void heightMap::affichage(){
    for(int l=0;l<1+pow(2,width);l++)
    {
        for(int c=0;c<1+pow(2,length);c++)
        {
            cout<< heightMatrix[l][c]<<"\t";
        }
        cout<<endl;
    }
}

void heightMap::diamondStep(){

}

void heightMap::squareStep(){

}

//CONSTRUCTORS
heightMap::heightMap()
{
     setLength(0);
     setWidth(0);
     setMaxDepth(0);
     setMaxHeight(0);

    for(int l=0;l<1000;l++) //By default the map's dimensions are 1000x1000
    {
        vector<int> row;
        for(int c=0;c<1000;c++)
        {
            row.push_back(0);
        }
        heightMatrix.push_back(row);
    }

}

heightMap::heightMap(int myLength, int myWidth, int myMaxDepth, int myMaxHeight)
{
     setLength(myLength);
     setWidth(myWidth);
     setMaxDepth(myMaxDepth);
     setMaxHeight(myMaxHeight);

    for(int l=0;l<1+pow(2,myWidth);l++)
    {
        vector<int> row;
        for(int c=0;c<1+pow(2,myLength);c++)
        {
            row.push_back(0);
        }
        heightMatrix.push_back(row);
    }
}

//DESTRUCTOR
heightMap::~heightMap()
{
    //dtor
}
