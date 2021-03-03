#pragma once

#include "Mesh.h"
#include "Shader.h"

#include <memory>

struct RenderInfo
{
	RenderInfo(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader)
		: Mesh(mesh), Shader(shader) {}

	std::shared_ptr<Mesh> Mesh;
	std::shared_ptr<Shader> Shader;
};