#include "Chrono.h"
#include <GL/glut.h>

Chrono::Chrono(void)
{
	last_time = glutGet(GLUT_ELAPSED_TIME);
	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
}


Chrono::~Chrono(void)
{
}

// Demarrer le chrono
void Chrono::Tic()
{
	current_time = glutGet(GLUT_ELAPSED_TIME);
	last_time = current_time;
}

// Arreter le chrono
void Chrono::Toc()
{
	current_time = glutGet(GLUT_ELAPSED_TIME);
	ellapsed_time = current_time - last_time;
}

int Chrono::getEllapsed_time()
{
	return ellapsed_time;
}