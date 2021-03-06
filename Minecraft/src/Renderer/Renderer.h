#pragma once

#include "glad/glad.h"

#include "Shader.h"

class Mesh;

#define ASSERT(x) if (!(x)) __debugbreak();

class Renderer
{
public:
    static void Clear();
    void Draw(Mesh& mesh, Shader& shader);
};
