#include "Chrono.h"
#include <GL/glut.h>

// CONSTRUCTOR
Chrono::Chrono(void)
{
	last_time = glutGet(GLUT_ELAPSED_TIME);
	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
}

// DESTRUCTOR
Chrono::~Chrono(void){}

// METHODS
// Demarrage du chrono
void Chrono::Tic()
{
	current_time = glutGet(GLUT_ELAPSED_TIME);
	last_time = current_time;
}

// Arret du chrono
void Chrono::Toc()
{
	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
}

int Chrono::getEllapsed_time()
{
	return ellapsed_time;
}