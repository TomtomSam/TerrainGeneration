#include "Point.h"

// CONSTRUCTORS
Point::Point()
{
	r = 1;
	g = 1;
	b = 1;
}

Point::Point(int _lig, int _col, float _height)
{
    lig = static_cast<float>(_lig);
    col = static_cast<float>(_col);
	height = _height;
}

// DESTRUCTOR
Point::~Point(){}

// GETTERS
float Point::getLig(){return lig;}
float Point::getCol(){return col;}
float Point::getHeight(){return height;}
float Point::getR(){return r;}
float Point::getG(){return g;}
float Point::getB(){return b;}
float Point::getS(){return s;}
float Point::getT(){return t;}

// SETTERS
void Point::setLig(float _lig){lig=_lig;}
void Point::setCol(float _col){col=_col;}
void Point::setHeight(float _height){height=_height;}
void Point::setR(float _r){r=_r;}
void Point::setG(float _g){g=_g;}
void Point::setB(float _b){b=_b;}
void Point::setS(float _s){s=_s;}
void Point::setT(float _t){t=_t;}
