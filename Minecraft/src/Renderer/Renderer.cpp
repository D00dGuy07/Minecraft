#include "Renderer.h"

#include <iostream>

#include "RenderInfo.h"

void Renderer::Clear()
{
    glClearColor(0.3f, 0.5f, 0.8f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(RenderInfo& renderInfo)
{
    renderInfo.Shader->Bind();
    unsigned int count = renderInfo.Mesh->Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    renderInfo.Mesh->Unbind();
    renderInfo.Shader->Unbind();
}
