#include "Program.h"

// CONSTRUCTOR
Program::Program(void){}

// DESTRUCTOR
Program::~Program(void){}

// METHODS
// Fonction d'ajout d'un shader a un program
void Program::addShaderToProgram(Shader* shader){
	glAttachShader(programID, shader->getShaderID());
}

// Fonction de lien entre deux shaders d'un program
void Program::linkProgram(Shader* shader1, Shader* shader2){
	createProgram();
	addShaderToProgram(shader1);
	addShaderToProgram(shader2);
	glLinkProgram(programID);
}

void Program::setUniformi(const GLchar* name, GLint value){
	GLint loc = glGetUniformLocation(programID, name);
	glUniform1i(loc, value);
}

void Program::setUniformf(const GLchar* name, GLfloat value){
	GLint loc = glGetUniformLocation(programID, name);
	glUniform1f(loc, value);
}

void Program::useProgram(){glUseProgram(programID);}
GLuint Program::getProgramID(){return programID;}
void Program::createProgram(){programID = glCreateProgram();}
void Program::deleteProgram(){glDeleteProgram(programID);}
