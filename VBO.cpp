#include "VBO.h"
using namespace std;

// FEEDERS
void VBO::FeedPos(vector<float> _pos){ pos.clear(); pos = _pos; }
void VBO::FeedCol(vector<float> _col){ col.clear(); col = _col; }
void VBO::FeedTex(vector<float> _tex){ tex.clear(); tex = _tex; }

// METHODS
// Fonction de construction du VBO
void VBO::BuildBuffer()
{
#define P_SIZE 3
#define C_SIZE 3
#define T_SIZE 2
#define BUFFER_OFFSET(a) ((char*)NULL + (a))

	/* POSITION */

	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferPos);

	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferPos);

	// Allocation l'espace necessaire en memoire (2^n strips à tracer et chaque strip contient 2*(2^n+1) sommets)
	glBufferData(GL_ARRAY_BUFFER,                   // Cible 
		pos.size() *sizeof pos[0],					// Taille des positions
		NULL,
		GL_STREAM_DRAW);							// Mode de dessin

	// Remplissage du buffer avec les subData
	if (pos.size() > 0)
	{
		// Specification des donnees
		glBufferSubData(GL_ARRAY_BUFFER,
			0,							// Emplacement des donnees dans le VBO
			pos.size()*sizeof pos[0],	// Taille des donnees
			&pos[0]);					// Adresse des donnes

		glVertexPointer(P_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);
	}

	/* COLORS */

	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferCol);

	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferCol);

	// Allocation l'espace necessaire en memoire
	glBufferData(GL_ARRAY_BUFFER,       // Cible 
		 col.size()*sizeof col[0],		// Taille des couleurs
		NULL,
		GL_STREAM_DRAW);				// Mode de dessin

	//Remplissage du buffer avec les subData
	if (col.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER,
			0,							// Emplacement
			col.size()*sizeof col[0],	// Taille
			&col[0]);					// Adresse

		glColorPointer(C_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_COLOR_ARRAY);
	}

	/* TEXTURES */

	// Creation d'un objet tampon et recuperation de son identifiant
	glGenBuffers(1, &bufferTex);

	// Bindage du buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferTex);

	// Allocation l'espace necessaire en memoire
	glBufferData(GL_ARRAY_BUFFER,       // Cible 
		tex.size()*sizeof tex[0],		//  Taille des textures
		NULL,
		GL_STREAM_DRAW);				// Mode de dessin

	//Remplissage du buffer avec les subData
	if (tex.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER,
			0,							// Emplacement
			tex.size()*sizeof tex[0],	// Taille
			&tex[0]);					// Adresse

		glClientActiveTexture(GL_TEXTURE0 + 2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(T_SIZE, GL_FLOAT, 0, BUFFER_OFFSET(0));
 
		glClientActiveTexture(GL_TEXTURE0);
	}
}

// Fonction de dessin du VBO
void VBO::DrawBuffer()
{
	// Dessin des strips
	glBindBuffer(GL_ARRAY_BUFFER, bufferPos);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, pos.size() / 3);
}

// Fonction de mise a jour des positions du VBO
void VBO::ActualizePosBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferPos);
	GLvoid *pos_vbo = NULL;

  //On obtient la position mémoire de nos data
	pos_vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// On transfert les nouvelles data au bon endroit 
	memcpy(pos_vbo, &pos[0], pos.size()*sizeof pos[0]);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	pos_vbo = NULL;
}

// Fonction de mise a jour des couleurs du VBO
void VBO::ActualizeColBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferCol);
	GLvoid *col_vbo = NULL;

	//On obtient la position mémoire de nos data
	col_vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// On transfert les nouvelles data au bon endroit 
	memcpy(col_vbo, &col[0], col.size()*sizeof col[0]);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	col_vbo = NULL;
}

void VBO::ActualizeTexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferTex);
	GLvoid *tex_vbo = NULL;

	//On obtient la position mémoire de nos data
	tex_vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	// On transfert les nouvelles data au bon endroit 
	memcpy(tex_vbo, &tex[0], tex.size()*sizeof tex[0]);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	tex_vbo = NULL;
}

// Fonction de liberation de la place du VBO en memoire
void VBO::DestroyVBO()
{
	// Desactivation des tableaux de sommets
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Liberation de la place en memoire
	glDeleteBuffers(1, &bufferPos);
	glDeleteBuffers(1, &bufferCol);
	glDeleteBuffers(1, &bufferTex);
}

// CONSTRUCTOR
VBO::VBO(){}

// DESTRUCTOR
VBO::~VBO(){}
