#ifndef PROGRAM_H
#define PROGRAM_H

#include "Shader.h"
class Program
{
public:
	Program(void);
	~Program(void);

   bool addShaderToProgram(Shader* shader);
   bool linkProgram();
   void useProgram();
   GLuint getProgramID();

private:
   GLuint uiProgram; // ID of program
};

#endif //PROGRAM_H
