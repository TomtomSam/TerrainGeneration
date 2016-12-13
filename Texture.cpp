#include "Texture.h"

// CONSTRUCTOR
Texture::Texture(void)
{
}

// DESTRUCTOR
Texture::~Texture(void)
{
}

// METHODS
// Fonction de chargement d'une texture a partir du nom du fichier image associe
void Texture::loadTexture(char* filename){
	texture = SOIL_load_OGL_texture(filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
		);
	if(texture == NULL){
		printf("Texture failed to load!\n");
	}
	else{
		printf("Texture loaded\n");
	}
}

// GETTERS
GLuint Texture::getTexture(){ return texture; }