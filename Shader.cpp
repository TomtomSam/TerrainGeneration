#include "Shader.h"

using namespace std;

Shader::Shader(void){}

Shader::~Shader(void){}

void Shader::createShaderID(GLenum shadertype){
	shaderID = glCreateShader(shadertype);
}

string Shader::readFile(const char *filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);

    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::loadShader(const char *path, GLenum shadertype){
	string ShaderStr = this->readFile(path);
	const char *ShaderSrc = ShaderStr.c_str();
	
	createShaderID(shadertype);

	glShaderSource(shaderID, 1, &ShaderSrc, NULL);
	glCompileShader(shaderID);
}

GLuint Shader::getShaderID()
{
	return shaderID;
}

void Shader::deleteShader()
{
	glDeleteShader(shaderID);
}
