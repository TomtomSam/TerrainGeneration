#ifndef POINT_H
#define POINT_H

class Point
{
    public:
        Point();
        Point(int _lig, int _col,float _height);
        virtual ~Point();

        float getLig();
        float getCol();
        float getHeight();
        float getR();
        float getG();
        float getB();
        float getS();
        float getT();

        void setLig(float _lig);
        void setCol(float _col);
        void setHeight(float _height);
        void setR(float _r);
        void setG(float _g);
        void setB(float _b);
        void setS(float _s);
        void setT(float _t);

    protected:

    private:
        float lig;
        float col;
        float height;

        float r;
        float g;
        float b;

        float s;
        float t;
};

#endif // POINT_H
