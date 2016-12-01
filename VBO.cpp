#include "VBO.h"
#include <vector>

using namespace std;

void VBO::FeedData(vector<float> _pos, vector<float> _col, vector<float> _tex)
{ 
	pos = _pos;
	col = _col;
	tex = _tex;
	
}


void VBO::BuildAndDrawBuffer()
{
	#define P_SIZE 3
	#define C_SIZE 3
	#define T_SIZE 2
	#define BUFFER_OFFSET(a) ((char*)NULL + (a))

	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &buffer);

	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// On alloue l'espace necessaire en memoire
	// Il y a 2^n strips à tracer et chaque strip contient 2*(2^n+1) sommets
	glBufferData(GL_ARRAY_BUFFER,                   // Cible 
		(pos.size() + col.size() + tex.size())*sizeof pos[0] ,	// Taille des positions Taille des couleurs Taille des textures
		NULL,
		GL_STREAM_DRAW);							// Mode de dessin

	//Remplissage du buffer avec les subData
	if (pos.size() > 0)
	{
		// Specification des donnees
		glBufferSubData(GL_ARRAY_BUFFER,
			0,                            // emplacement des donnees dans le VBO
			pos.size()*sizeof pos[0], // Taille des donnees
			&pos[0]);

		glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);
	}


	if (col.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER,
			pos.size()*sizeof pos[0],	// Emplacement
			col.size()*sizeof pos[0],	// Taille
			&col[0]);							// Adresse

		glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(pos.size()*sizeof pos[0]));
		glEnableClientState(GL_COLOR_ARRAY);
	}

	if (tex.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER,
			pos.size()*sizeof pos[0] + col.size()*sizeof pos[0],	// Emplacement
			tex.size()*sizeof pos[0],	// Taille
			&tex[0]);							// Adresse
		
		glTexCoordPointer(T_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(pos.size()*sizeof pos[0] + col.size()*sizeof pos[0]));
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	// Dessin des strips
	glDrawArrays(GL_TRIANGLE_STRIP, 0, pos.size()/3);

	// Desactivation des tableaux de sommets
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Liberation de la place en memmoire
	glDeleteBuffers(1, &buffer);
}

VBO::VBO()
{
	pos.clear();
	tex.clear();
	col.clear();
}


VBO::~VBO()
{
}
