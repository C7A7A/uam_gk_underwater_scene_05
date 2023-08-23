#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator()
{

}


void ParticleGenerator::initGenerator() {

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];


	for (int i = 0; i < MaxParticles; i++) {
		particlesContainer[i].life = -1.0f;
		particlesContainer[i].cameradistance = -1.0f;
	}


	particleTexture = Core::LoadTexture("textures/particle.png");

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Initialize with empty (NULL) buffer : it will be updated later, each frame.

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Initialize with empty (NULL) buffer : it will be updated later, each frame.

	glBindVertexArray(0);
}

void ParticleGenerator::drawGenerator(glm::vec3 position, float delta, Camera camera, GLuint programParticles) {

	// Generate 1 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	int newparticles = (int)(delta * 500.0);
	if (newparticles > (int)(0.016f * 500.0))
		newparticles = (int)(0.016f * 500.0);

	for (int i = 0; i < newparticles; i++) {
		int particleIndex = findUnusedParticle();
		particlesContainer[particleIndex].life = 15.0f; // This particle will live 10 seconds.
		particlesContainer[particleIndex].pos = position;

		float spread = 1.8f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
		
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		particlesContainer[particleIndex].speed = maindir + randomdir * spread;

		// Very bad way to generate a random color
		particlesContainer[particleIndex].r = randNumber(150, 250);
		particlesContainer[particleIndex].g = randNumber(150, 250);
		particlesContainer[particleIndex].b = 255;
		particlesContainer[particleIndex].a = (rand() % 256) / 3;

		particlesContainer[particleIndex].size = (rand() % 1000) / 1500.0f + 0.1f;

	}



	// Simulate all particles
	int ParticlesCount = 0;
	for (int i = 0; i < MaxParticles; i++) {

		Particle& p = particlesContainer[i]; // shortcut

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f) {

				// Simulate simple physics : gravity only, no collisions
				p.speed += glm::vec3(0.0f, -1.0f, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta / 4;
				p.cameradistance = glm::length2(p.pos - camera.cameraPos);

				// Fill the GPU buffer
				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
				g_particule_color_data[4 * ParticlesCount + 3] = p.a;

			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		}
	}

	sortParticles();



	// Update the buffers that OpenGL uses for rendering.
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// shader
	glUseProgram(programParticles);

	glm::mat4 ViewProjectionMatrix = camera.perspectiveMatrix * camera.cameraMatrix;

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(programParticles, "myTextureSampler"), 0);

	glUniform3f(glGetUniformLocation(programParticles, "CameraRight_worldspace"), camera.cameraMatrix[0][0], camera.cameraMatrix[1][0], camera.cameraMatrix[2][0]);
	glUniform3f(glGetUniformLocation(programParticles, "CameraUp_worldspace"), camera.cameraMatrix[0][1], camera.cameraMatrix[1][1], camera.cameraMatrix[2][1]);

	glUniformMatrix4fv(glGetUniformLocation(programParticles, "VP"), 1, GL_FALSE, &ViewProjectionMatrix[0][0]);


	glBindVertexArray(VertexArrayID);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	// Draw the particles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);

	glUseProgram(0);

}

void ParticleGenerator::clearGenerator() {

	delete[] g_particule_position_size_data;
	delete[] g_particule_color_data;
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}


int ParticleGenerator::findUnusedParticle() {

	for (int i = lastUsedParticle; i < MaxParticles; i++) {
		if (particlesContainer[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (particlesContainer[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void ParticleGenerator::sortParticles() {
	std::sort(&particlesContainer[0], &particlesContainer[MaxParticles]);
}
