#include "Renderer.h"

#include <iostream>

#include "Mesh.h"

void Renderer::Clear()
{
    glClearColor(0.3f, 0.5f, 0.8f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(Mesh& mesh, Shader& shader)
{
    shader.Bind();
    unsigned int count = mesh.Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    mesh.Unbind();
    shader.Unbind();
}
