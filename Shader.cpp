#include "Shader.h"
using namespace std;

// CONSTRUCTOR
Shader::Shader(void){}

// DESTRUCTOR
Shader::~Shader(void){}

// METHODS
// Fonction de creation un shader du type donne
void Shader::createShaderID(GLenum shadertype){
	shaderID = glCreateShader(shadertype);
}

// Fonction de lecture du contenu d'un fichier texte
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

// Fonction de chargement d'un shader
void Shader::loadShader(const char *path, GLenum shadertype){
	string ShaderStr = this->readFile(path);
	const char *ShaderSrc = ShaderStr.c_str();
	
	createShaderID(shadertype);

	glShaderSource(shaderID, 1, &ShaderSrc, NULL);
	glCompileShader(shaderID);
}

// Fonction de liberation de la memoire consommee par le shader
void Shader::deleteShader()
{
	glDeleteShader(shaderID);
}

// GETTERS
GLuint Shader::getShaderID()
{
	return shaderID;
}
