#pragma once
#include "objload.h"
#include "glew.h"
#include <freeglut.h>
#include <ext.hpp>
#include "glm.hpp"
#include "../helpers/Random.h"
#include "Camera.h"
#include "Texture.h"

class ParticleGenerator {

public:
	ParticleGenerator();
	void initGenerator();
	void drawGenerator(glm::vec3 position, float delta, Camera camera, GLuint programParticles);
	void clearGenerator();

private:
	int findUnusedParticle();
	void sortParticles();

public:
	unsigned int  VertexArrayID = 0, billboard_vertex_buffer = 0, particles_color_buffer = 0, particles_position_buffer = 0;
	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
	GLuint particleTexture = 0;

	glm::vec3 particlesPosition = glm::vec3(0);

private:
	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life; // Remaining life of the particle. if <0 : dead and unused.
		float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

		bool operator<(const Particle& that) const {
			// Sort in reverse order : far particles drawn first.
			return this->cameradistance > that.cameradistance;
		}
	};

	static const int MaxParticles = 10000;
	Particle particlesContainer[MaxParticles];
	int lastUsedParticle = 0;

};
