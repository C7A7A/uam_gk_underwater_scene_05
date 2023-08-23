#include "Skybox.h"

//#define STB_IMAGE_IMPLEMENTATION

Skybox::Skybox()
{
}

void Skybox::prepareSkybox() {
	prepareSkyboxData();
	createSkyboxTexture();
	attachTextureToCubemap();
}

void Skybox::drawSkybox(Camera camera, GLuint program) {
	// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
	glDepthFunc(GL_LEQUAL);

	glUseProgram(program);

	// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
	// The last row and column affect the translation of the skybox (which we don't want to affect)
	glm::mat4 view = glm::mat4(glm::mat3(camera.cameraMatrix));
	glm::mat4 projection = camera.perspectiveMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

	// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
	// where an object is present (a depth of 1.0f will always fail against any object's depth value)
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Switch back to the normal depth function
	glDepthFunc(GL_LESS);
}

void Skybox::prepareSkyboxData() {
	// VAO, VBO, EBO for the skybox
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::createSkyboxTexture() {
	// Create cubemap texture object
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::attachTextureToCubemap() {
	// Cycle through all the textures and attach them to the cubemap object
	for (unsigned int i = 0; i < 6; i++) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(cubemapSides[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture: " << cubemapSides[i] << std::endl;
			stbi_image_free(data);
		}
	}
}
