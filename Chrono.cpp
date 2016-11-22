#include "Chrono.h"


Chrono::Chrono(void)
{
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