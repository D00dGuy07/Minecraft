#pragma once

#include "glad/glad.h"

#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();

struct RenderInfo;

class Renderer
{
public:
    static void Clear();
    void Draw(RenderInfo& renderInfo);
};
