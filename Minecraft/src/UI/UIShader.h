#pragma once

#include <string>

class Shader;

struct UIShaderSource
{
	std::string Shared;
	std::string Instanced;
	std::string Single;
	std::string Fragment;
	std::string FilePath;
};

class UIShader
{
public:
	UIShader(const std::string& filePath, bool binary);
	~UIShader();

	inline Shader* GetInstanced() const { return reinterpret_cast<Shader*>(m_InstancedShader); }
	inline Shader* GetSingle() const { return reinterpret_cast<Shader*>(m_SingleShader); }

	void WriteBinary(const std::string& filePath);
private:
	std::string m_FilePath;

	Shader* m_InstancedShader;
	Shader* m_SingleShader;

	static UIShaderSource ParseShader(const std::string& filePath);
};