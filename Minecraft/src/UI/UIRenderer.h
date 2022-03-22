#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <stdint.h>

// Forward decl
class Mesh;
class Framebuffer;
class Shader;
class UILayout;
class UIShader;

struct UIInstancingSpec
{
	float ZIndex;
	float Rotation;
	glm::vec2 Scale;
	glm::vec2 Position;

	bool Transparent;

	size_t DataSize;
	uint8_t* Data;

	UIInstancingSpec(float zIndex, float rotation, glm::vec2 scale, glm::vec2 position, bool transparent, size_t extra);
};

class UIRenderer
{
public:
	UIRenderer();
	~UIRenderer();

	void Render(UILayout* layout);

	void AddQuad(const std::string& shader, const UIInstancingSpec& instance);

	inline Framebuffer* GetFramebuffer() const { return m_UIBuffer; }

private:

	Framebuffer* m_UIBuffer;
	std::unordered_map<std::string, std::vector<UIInstancingSpec>> m_InstanceQueues;
	std::vector<std::pair<std::string, UIInstancingSpec>> m_TransparentQueue;

	void LoadShader(const std::string& name);
	void ClearQueues();

	// Quad mesh can be shared between all instances of UIRenderer so 
	// m_Refs keeps track of how many instances there are so that the mesh 
	// can be disposed of correctly when it's not being used anymore
	static int32_t m_Refs;
	static std::unordered_map<std::string, UIShader*> m_Shaders;
	static Mesh* m_Quad;

	static void InitializeResources();
	static void FreeResources();
};