#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

class Shader
{
public:
	Shader(void);
	~Shader(void);

	bool LoadShader(string sFile, int a_iType);
	void DeleteShader();
	bool GetLinesFromFile(string sFile, bool bIncludePart, vector<string>* vResult);
	bool IsLoaded();
	GLuint GetShaderID();

private:
	GLuint shaderID;
	int type; // GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	bool loaded; // Whether shader was loaded and compiled
};

#endif //SHADER_H