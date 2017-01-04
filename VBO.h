#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <vector>
using namespace std;

class VBO
{

private:

	//Le vbo
	GLuint bufferPos;
	GLuint bufferCol;
	GLuint bufferTex;

	//Les vecteurs de data
	vector<float> pos;
	vector<float> col;
	vector<float> tex;

public:

	void FeedPos(vector<float> _pos); 
	void FeedCol(vector<float> _col);
	void FeedTex(vector<float> _tex);
	GLuint getBufferPos(); // MaJ/////////////////////////////////////////////////////////////
	GLuint getBufferCol();
	GLuint getBufferTex();

	//void BuildAndDrawBuffer();
	void BuildBuffer();
	void DrawBuffer();
	void ActualizePosBuffer();
	void ActualizeColBuffer();
	void ActualizeTexBuffer();
	void DestroyVBO();


	VBO();
	~VBO();
};

#endif //VBO_H