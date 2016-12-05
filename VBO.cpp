#include "VBO.h"
#include <vector>

using namespace std;


void VBO::FeedPos(vector<float> _pos){ pos = _pos; }
void VBO::FeedCol(vector<float> _col){ col = _col; }
void VBO::FeedTex(vector<float> _tex){ tex = _tex; }

void VBO::BuildBuffer()
{
#define P_SIZE 3
#define C_SIZE 3
#define T_SIZE 2
#define BUFFER_OFFSET(a) ((char*)NULL + (a))


	//POSITION

	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferPos);
	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferPos);
	// On alloue l'espace necessaire en memoire
	// Il y a 2^n strips à tracer et chaque strip contient 2*(2^n+1) sommets
	glBufferData(GL_ARRAY_BUFFER,                   // Cible 
		pos.size() *sizeof pos[0],	// Taille des positions Taille des couleurs Taille des textures
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


	//COLORS
	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferCol);

	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferCol);

	// On alloue l'espace necessaire en memoire
	// Il y a 2^n strips à tracer et chaque strip contient 2*(2^n+1) sommets
	glBufferData(GL_ARRAY_BUFFER,                   // Cible 
		 col.size()*sizeof col[0],	// Taille des positions Taille des couleurs Taille des textures
		NULL,
		GL_STREAM_DRAW);							// Mode de dessin

	//Remplissage du buffer avec les subData
	if (col.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER,
			0,	// Emplacement
			col.size()*sizeof pos[0],	// Taille
			&col[0]);					// Adresse

		glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_COLOR_ARRAY);
	}

	//TEXTURES
	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferTex);
	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferTex);
	// On alloue l'espace necessaire en memoire
	// Il y a 2^n strips à tracer et chaque strip contient 2*(2^n+1) sommets
	glBufferData(GL_ARRAY_BUFFER,                   // Cible 
		tex.size()*sizeof col[0],	// Taille des positions Taille des couleurs Taille des textures
		NULL,
		GL_STREAM_DRAW);							// Mode de dessin

	if (tex.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER,
			0,	// Emplacement
			tex.size()*sizeof pos[0],	// Taille
			&tex[0]);							// Adresse

		glTexCoordPointer(T_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

}
void VBO::DrawBuffer()
{
	// Dessin des strips
	glBindBuffer(GL_ARRAY_BUFFER, bufferPos);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, pos.size() / 3);
	
}

void VBO::ActualizePosBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferPos);
	GLvoid *pos_vbo = NULL;

	//On obtient la position mémoire de nos data
	pos_vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (pos_vbo == NULL)

	{

		fprintf(stderr, "impossible d'acceder aux donnees du vbo!\n");

		exit(EXIT_FAILURE);

	}


	// On transfert les nouvelles data au bon endroit 
	memcpy(pos_vbo, &pos[0], pos.size()*sizeof pos[0]);


	glUnmapBuffer(GL_ARRAY_BUFFER);

	pos_vbo = NULL;
}

void VBO::ActualizeColBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferCol);
	GLvoid *col_vbo = NULL;

	//On obtient la position mémoire de nos data
	col_vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	if (col_vbo == NULL)

	{

		fprintf(stderr, "impossible d'acceder aux donnees du vbo!\n");

		exit(EXIT_FAILURE);

	}


	// On transfert les nouvelles data au bon endroit 
	memcpy(col_vbo, &col[0], col.size()*sizeof col[0]);


	glUnmapBuffer(GL_ARRAY_BUFFER);

	col_vbo = NULL;
}

void VBO::DestroyVBO()
{
	// Desactivation des tableaux de sommets
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Liberation de la place en memmoire
	glDeleteBuffers(1, &bufferPos);
	glDeleteBuffers(1, &bufferCol);
	glDeleteBuffers(1, &bufferTex);
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


/*void VBO::BuildAndDrawBuffer()
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
}*/
