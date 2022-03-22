#include "UI/UIShader.h"

#include "glad/glad.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

#include <fstream>
#include <sstream>

// This whole class is just an interface with shader base
// I'm not sure how I feel about this
class UIShaderInterface : public ShaderBase
{
public:
	UIShaderInterface(const std::string& filePath)
		: ShaderBase(), m_FilePath(filePath) { m_RendererID = NULL; }

	static void CreateUIShaders(const UIShaderSource& source, UIShaderInterface* single, UIShaderInterface* instanced)
	{
		// Create program objects and compile shader source
		single->m_RendererID = glCreateProgram();
		instanced->m_RendererID = glCreateProgram();

		uint32_t sharedBin = CompileShader(GL_VERTEX_SHADER, source.Shared, source.FilePath + " (Shared Shader)");
		uint32_t singleBin = CompileShader(GL_VERTEX_SHADER, source.Single, source.FilePath + " (Single Shader)");
		uint32_t instancedBin = CompileShader(GL_VERTEX_SHADER, source.Instanced, source.FilePath + " (Instanced Shader)");
		uint32_t fragmentBin = CompileShader(GL_FRAGMENT_SHADER, source.Fragment, source.FilePath + " (Fragment Shader)");

		// Attach shaders and link for single shader
		glAttachShader(single->m_RendererID, singleBin);
		glAttachShader(single->m_RendererID, sharedBin);
		glAttachShader(single->m_RendererID, fragmentBin);
		glLinkProgram(single->m_RendererID);
		glValidateProgram(single->m_RendererID);

		// Attach shaders and link for instanced shader
		glAttachShader(instanced->m_RendererID, instancedBin);
		glAttachShader(instanced->m_RendererID, sharedBin);
		glAttachShader(instanced->m_RendererID, fragmentBin);
		glLinkProgram(instanced->m_RendererID);
		glValidateProgram(instanced->m_RendererID);

		// Cleanup shader binary
		glDeleteShader(sharedBin);
		glDeleteShader(singleBin);
		glDeleteShader(instancedBin);
		glDeleteShader(fragmentBin);
	}

	static void LoadShaderBinary(const std::string& filePath, UIShaderInterface* single, UIShaderInterface* instanced)
	{
		// Load binary data from file
		std::ifstream file(filePath, std::ios::binary);
		if (!file.good())
			return;

		ShaderBinary singleBin = ShaderBinary::LoadStream(file);
		ShaderBinary instancedBin = ShaderBinary::LoadStream(file);
		file.close();

		// Create shader objects
		single->CreateFromBinary(singleBin);
		instanced->CreateFromBinary(instancedBin);

		// Clean up binary data
		singleBin.Free();
		instancedBin.Free();
	}
private:
	// File path is here so that this can be cast to a regular shader cleanly
	std::string m_FilePath;

	// This function has to exist because the LoadBinary member function is protected
	void CreateFromBinary(ShaderBinary binary)
	{
		m_RendererID = glCreateProgram();
		LoadBinary(binary);
	}
};

UIShader::UIShader(const std::string& filePath, bool binary)
	: m_FilePath(filePath), 
	m_SingleShader(reinterpret_cast<Shader*>(new UIShaderInterface(filePath))), 
	m_InstancedShader(reinterpret_cast<Shader*>(new UIShaderInterface(filePath)))
{
	if (binary)
	{
		Renderer::Submit([&]() {
			UIShaderInterface::LoadShaderBinary(m_FilePath, 
				reinterpret_cast<UIShaderInterface*>(m_SingleShader), 
				reinterpret_cast<UIShaderInterface*>(m_InstancedShader));
		});
	}
	else
	{
		Renderer::Submit([&]() {
			UIShaderInterface::CreateUIShaders(ParseShader(m_FilePath), 
				reinterpret_cast<UIShaderInterface*>(m_SingleShader),
				reinterpret_cast<UIShaderInterface*>(m_InstancedShader));
		});
	}
}

UIShader::~UIShader()
{
	delete m_InstancedShader;
	delete m_SingleShader;
}

void UIShader::WriteBinary(const std::string& filePath)
{
	// Opening file before getting binary data in case it fails
	std::ofstream file(filePath, std::ios::trunc | std::ios::binary);
	if (!file.good())
		return;

	ShaderBinary single = m_SingleShader->GetBinary();
	ShaderBinary instanced = m_InstancedShader->GetBinary();

	single.WriteStream(file);
	instanced.WriteStream(file);

	single.Free();
	instanced.Free();
}

UIShaderSource UIShader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, SHARED = 0, INSTANCED = 1, SINGLE = 2, FRAGMENT = 3
	};

	std::string line;
	std::stringstream ss[4];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("shared") != std::string::npos)
				type = ShaderType::SHARED;
			else if (line.find("instanced") != std::string::npos)
				type = ShaderType::INSTANCED;
			else if (line.find("single") != std::string::npos)
				type = ShaderType::SINGLE;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str(), ss[2].str(), ss[3].str(), filePath };
}