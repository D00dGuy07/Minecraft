#pragma once

#include <unordered_map>
#include <vector>
#include <string>

class UIComponent;
class UIRenderer;

struct UIComponentData
{
	UIComponent* Component = NULL;
	bool Updated;
	std::vector<std::string> Dependencies;
};

class UILayout
{
public:
	UILayout() {}
	~UILayout();

	void AddComponent(UIComponent* component);
	UIComponent* GetComponent(const std::string& id);

	void Resize(int width, int height);
	void Verify();

	bool FillRenderQueues(UIRenderer* renderer);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	inline uint32_t GetZRange() const { return m_ZRange; }
	inline void SetZRange(uint32_t range) { m_ZRange = range; }

	inline void ForceRender() { m_ForceRender = true; }

private:
	int m_Width = 0, m_Height = 0;
	uint32_t m_ZRange = 100;
	bool m_ForceRender = false;
	std::unordered_map<std::string, UIComponentData> m_Components = std::unordered_map<std::string, UIComponentData>();

	UIComponentData* GetComponentData(const std::string& id);
	void RecursiveVerify(const std::string& id, std::vector<std::string> dependencies);
	void RecursiveUpdate(const std::string& id);
};

// Exceptions

class LayoutCircularDependency : public std::exception
{
public:
	LayoutCircularDependency(const std::string& message)
		: std::exception(message.c_str()) {}
};

class NonexistentComponentError : public std::exception
{
public:
	NonexistentComponentError(const std::string& message, const std::string& id)
		: std::exception(message.c_str()), ID(id) {}

	const std::string ID;
};