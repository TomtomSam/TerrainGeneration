#include "Texture.h"


Texture::Texture(void)
{
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
		printf("Texture failed to load!\n");
	}
	else{
		printf("Texture loaded\n");
	}
}

GLuint Texture::getTexture(){ return texture; }