#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <vector>
using namespace std;

class VBO
{

private:

	//Le vbo
	GLuint buffer;

	//Les vecteurs de data
	vector<float> pos;
	vector<float> col;
	vector<float> tex;

public:

	void FeedData(vector<float> _pos, vector<float> _col, vector<float> _tex);

	void BuildAndDrawBuffer();


	VBO();
	~VBO();
};

#endif //VBO_H