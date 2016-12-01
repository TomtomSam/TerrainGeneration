#ifndef COLORRAMP_H
#define COLORRAMP_H

#include <vector>
using namespace std;

class ColorRamp
{

private:
	vector<float> R;
	vector<float> G;
	vector<float> B;


public:
	void Remplissage(float HauteurMax,float posOcean);
	vector<float> Lerp(float col1, float col2, int env);

	float getR(int i);
	float getG(int i);
	float getB(int i);
	ColorRamp();
	~ColorRamp();
};

vector<float>& operator+=(vector<float>& v1, vector<float>& v2);


#endif // COLORRAMP_H