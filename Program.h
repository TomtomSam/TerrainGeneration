#ifndef PROGRAM_H
#define PROGRAM_H

#include "Shader.h"
class Program
{
public:
	Program(void);
	~Program(void);

	void addShaderToProgram(Shader* shader);
	void linkProgram(Shader* shader1, Shader* shader2);
	void useProgram();
	GLuint getProgramID();
	void createProgram();
	void deleteProgram();



private:
	GLuint programID; // ID of program
};

#endif //PROGRAM_H

