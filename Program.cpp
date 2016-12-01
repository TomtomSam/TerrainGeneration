#include "Program.h"


Program::Program(void)
{
	uiProgram = glCreateProgram();
}


Program::~Program(void)
{
}

bool Program::addShaderToProgram(Shader* shader){return true;}

bool Program::linkProgram(){return true;}

void Program::useProgram(){}

GLuint Program::getProgramID(){return 1;}