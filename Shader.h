#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Shader
{
public:
	Shader(void);
	~Shader(void);

	//void loadShader(string sFile, GLenum a_iType);
	void deleteShader();
	GLuint getShaderID();
	void createShaderID(GLenum shader_type);
	string readFile(const char *filePath);
	void loadShader(const char *path, GLenum shadertype);

private:
	GLuint shaderID;
	GLenum type; // GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
};

#endif //SHADER_H