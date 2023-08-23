#pragma once

#include "glew.h"
#include "freeglut.h"

namespace Core
{
	GLuint LoadTexture(const char * filepath);

	// textureID - texture identificator returned by LoadTexture
	// shaderVariableName - name of the variable of type smapler2D used on the shader side
	// programID - program ID currently used on the side of the GPU
	// textureUnit - unit index of textures - a number from 0 to 7. Up to 8 textures can be used at once in a single shader sampler2D data structure.
	void SetActiveTexture(GLuint textureID, const char * shaderVariableName, GLuint programID, int textureUnit);
}