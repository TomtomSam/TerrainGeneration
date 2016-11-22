#ifndef TEX_H
#define TEX_H

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>
#include <fstream>
#include "SOIL.h"

using namespace std;

class Texture
{
public:
	Texture();
	virtual ~Texture();
	void loadTexture(char* filename);
	GLuint getTexture();

private:
    GLuint texture;      
}; 

#endif //TEX_H

