#include "UIRenderer.h"

// Renderer includes
#include "Renderer/Renderer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/ShaderBuffer.h"

#include "UI/UILayout.h"
#include "UI/UIShader.h"

#include "Util/Sort.h"

#include "glm/ext/matrix_clip_space.hpp"

#include <iostream>

int32_t UIRenderer::m_Refs = 0;
std::unordered_map<std::string, UIShader*> UIRenderer::m_Shaders = std::unordered_map<std::string, UIShader*>();
Mesh* UIRenderer::m_Quad = NULL;


constexpr const char* ShaderPath = "res/ui/shaders/";

constexpr size_t SpecSize = sizeof(glm::vec2) * 2 + sizeof(float) * 2;

UIInstancingSpec::UIInstancingSpec(float zIndex, float rotation, glm::vec2 scale, glm::vec2 position, bool transparent, size_t extra)
	: ZIndex(zIndex), Rotation(rotation), Scale(scale), Position(position), Transparent(transparent), DataSize(extra), Data(NULL) 
{
	if (DataSize > 0)
		Data = reinterpret_cast<uint8_t*>(malloc(DataSize));
}

// Renderer

UIRenderer::UIRenderer()
	: m_UIBuffer(NULL), m_InstanceQueues(), m_TransparentQueue()
{
	if (m_Refs++ == 0)
		InitializeResources();
}

UIRenderer::~UIRenderer()
{
	delete m_UIBuffer;
	if (--m_Refs == 0)
		FreeResources();

	ClearQueues();
}

size_t FlattenInstanceData(void* destination, const UIInstancingSpec& instance)
{
	uint8_t* cursor = reinterpret_cast<uint8_t*>(destination);
	memcpy(cursor, &instance.ZIndex, sizeof(float));
	cursor += sizeof(float);
	memcpy(cursor, &instance.Rotation, sizeof(float));
	cursor += sizeof(float);
	memcpy(cursor, &instance.Scale, sizeof(glm::vec2));
	cursor += sizeof(glm::vec2);
	memcpy(cursor, &instance.Position, sizeof(glm::vec2));
	cursor += sizeof(glm::vec2);
	if (instance.DataSize > 0)
	{
		memcpy(cursor, instance.Data, instance.DataSize);
		cursor += instance.DataSize;
	}

	return reinterpret_cast<uint64_t>(cursor) - reinterpret_cast<uint64_t>(destination);
}

void UIRenderer::Render(UILayout* layout)
{
	bool shouldRender = layout->FillRenderQueues(this);
	if (!shouldRender && layout->GetWidth() != 0 && layout->GetHeight() != 0)
		return;

	// Prepare the framebuffer to render this layout
	if (m_UIBuffer == NULL)
	{
		m_UIBuffer = new Framebuffer({
			static_cast<uint32_t>(layout->GetWidth()), static_cast<uint32_t>(layout->GetHeight()),
			{{ImageBufferType::Texture, FBOAttachmentType::Color, InternalImageFormat::RGBA8}},
			{{ImageBufferType::RenderBuffer, FBOAttachmentType::DepthStencil, InternalImageFormat::Depth24_Stencil8}}
		});
	}
	else if (m_UIBuffer->GetWidth() != layout->GetWidth() || m_UIBuffer->GetHeight() != layout->GetHeight())
		m_UIBuffer->Resize(layout->GetWidth(), layout->GetHeight());

	// Render all of the opaque objects
	m_UIBuffer->Bind();
	Renderer::Clear();

	float zRange = static_cast<float>(layout->GetZRange());

	for (auto& pair : m_InstanceQueues)
	{
		// First step is to flatten the buffer, each spec can store a pointer to 
		// additional data and that needs to be added in at the end of the required data
		void* dataBuffer;
		size_t bufferSize = (SpecSize + pair.second[0].DataSize) * static_cast<uint32_t>(pair.second.size());
		if (bufferSize == 0)
			continue;
		else
		{
			// Fill the buffer
			dataBuffer = malloc(bufferSize);
			uint8_t* cursor = reinterpret_cast<uint8_t*>(dataBuffer);
			for (UIInstancingSpec& spec : pair.second)
			{
				spec.ZIndex = spec.ZIndex / zRange;
				cursor += FlattenInstanceData(cursor, spec);
			}
		}

		ShaderBuffer* shaderBuffer = Renderer::CreateGarbage<ShaderBuffer>(dataBuffer, bufferSize);
		shaderBuffer->BindIndexed(0);
		free(dataBuffer);

		Shader* uiShader = m_Shaders[pair.first]->GetInstanced();
		uiShader->Bind();
		uiShader->SetUniform1i("u_InstanceSize", (SpecSize + pair.second[0].DataSize) / static_cast<uint32_t>(sizeof(float)), false);
		uiShader->SetUniform2f("u_Resolution", glm::vec2(layout->GetWidth(), layout->GetHeight()), false);
		
		Renderer::SubmitMeshInstanced(*m_Quad, *uiShader, static_cast<uint32_t>(pair.second.size()));
	}

	// Render transparent objects
	uint32_t length = m_TransparentQueue.size();
	std::vector<uint32_t> sortedIndices(length);
	for (uint32_t i = 0; i < length; i++)
		sortedIndices[i] = i;

	// The sorting is actually happening on an array indices and not the actual
	// instancing data structures
	Sorting::Sort<Sorting::Algorithm::Introsort, uint32_t>(sortedIndices, 0, length - 1,
		[&](const uint32_t& a, const uint32_t& b) {
			return m_TransparentQueue[a].second.ZIndex < m_TransparentQueue[b].second.ZIndex;
		}
	);

	for (uint32_t index : sortedIndices)
	{
		auto& pair = m_TransparentQueue[index];

		// Flatten data just like with the instanced quads but instead of getting
		// sent to a shader buffer it gets sent to a uniform
		float* dataBuffer = NULL;
		size_t bufferSize = SpecSize + pair.second.DataSize;
		if (bufferSize == 0)
			continue;
		else
		{
			// Fill the buffer
			dataBuffer = reinterpret_cast<float*>(malloc(bufferSize));

			pair.second.ZIndex = pair.second.ZIndex / zRange;
			FlattenInstanceData(dataBuffer, pair.second);
		}

		Shader* uiShader = m_Shaders[pair.first]->GetSingle();
		uiShader->Bind();
		uiShader->SetUniform1fv("u_InstanceData", dataBuffer, static_cast<uint32_t>(bufferSize / sizeof(float)), false);
		uiShader->SetUniform2f("u_Resolution", glm::vec2(layout->GetWidth(), layout->GetHeight()), false);

		Renderer::SubmitMesh(*m_Quad, *uiShader);
	}

	m_UIBuffer->Unbind();
	ClearQueues();
}

void UIRenderer::AddQuad(const std::string& shader, const UIInstancingSpec& instance)
{
	if (instance.Transparent)
	{
		// Only way to check here if the shader needs to be loaded is by checking
		// m_Shaders which would be redundant since LoadShader already does that
		LoadShader(shader);

		m_TransparentQueue.push_back(std::make_pair(shader, instance));
	}
	else
	{
		// Load shader only in this if statement works because there will only be
		// a vector under the shader name if the shader exists
		if (m_InstanceQueues.find(shader) == m_InstanceQueues.end())
		{
			m_InstanceQueues[shader] = std::vector<UIInstancingSpec>();
			LoadShader(shader);
		}

		m_InstanceQueues[shader].push_back(instance);
	}
}

void UIRenderer::LoadShader(const std::string& name)
{
	if (m_Shaders.find(name) == m_Shaders.end())
		m_Shaders[name] = new UIShader(std::string(ShaderPath) + name, false);
}

void UIRenderer::ClearQueues()
{
	for (auto& pair : m_InstanceQueues)
	{
		for (UIInstancingSpec& spec : pair.second)
			free(spec.Data);
		pair.second.clear();
	}

	for (auto& pair : m_TransparentQueue)
		free(pair.second.Data);
	m_TransparentQueue.clear();
}

void UIRenderer::InitializeResources()
{
	m_Shaders = std::unordered_map<std::string, UIShader*>();

	// Create quad mesh, vertex positions can be used as UV coords as well
	glm::vec2 vertices[] = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	// Indices are flipped here because it gets flipped in the shader
	uint32_t indices[] = {
		2, 1, 0,
		0, 3, 2
	};

	m_Quad = new Mesh();
	m_Quad->SetVertices(reinterpret_cast<float*>(vertices), 8 * 4);
	m_Quad->SetIndices(indices, 6);
	m_Quad->BufferLayout.Push<float>(2);
	m_Quad->Construct();
}

void UIRenderer::FreeResources()
{
	for (auto& pair : m_Shaders)
		delete pair.second;

	delete m_Quad;
}