#include "terrainTexture.h"

terrainTexture::terrainTexture(std::string fileName) : filePath(fileName) {
	this->textureID = load();
}

void terrainTexture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void terrainTexture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int terrainTexture::load() {
	unsigned int id;
	glGenTextures(1, &id);

	this->data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
	if(data) {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
		glGenerateMipmap(GL_TEXTURE_2D); 

		// Set the class variables to the respective size of the image retrieved form stbi lib
		this->width = width;
		this->height = height;
		
		std::cout << "Loaded texture of size " << height << " x " << width << std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//stbi_image_free(data);
	} else {
		std::cout << "Texture failed to load at path: " << filePath << std::endl;
	}

	return id;
}
