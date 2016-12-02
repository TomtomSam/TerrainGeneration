#include "Program.h"

Program::Program(void)
{
	programID = glCreateProgram();
}


Program::~Program(void)
{
}

void Program::addShaderToProgram(Shader* shader){
	glAttachShader(programID, shader->getShaderID());
}

void Program::linkProgram(Shader* shader1, Shader* shader2){
	createProgram();
	addShaderToProgram(shader1);
	addShaderToProgram(shader2);
	glLinkProgram(programID);
}

void Program::useProgram(){glUseProgram(programID);}

GLuint Program::getProgramID(){return programID;}

void Program::createProgram(){programID = glCreateProgram();}
void Program::deleteProgram(){glDeleteProgram(programID);}

