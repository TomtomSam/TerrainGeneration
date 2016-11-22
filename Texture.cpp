#include "Texture.h"


Texture::Texture(void)
{
	texture = 0;
}


Texture::~Texture(void)
{
}

void Texture::loadTexture(char* filename){
	texture = SOIL_load_OGL_texture(filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
		);
	if(texture == NULL){
		printf("[Texture loader] \"%s\" failed to load!\n", filename);
	}
	else{
		printf("Texture loaded :", filename);
	}
}

GLuint Texture::getTexture()
{
	return texture;
}