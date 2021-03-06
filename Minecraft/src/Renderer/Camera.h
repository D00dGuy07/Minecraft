#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct GLFWwindow;

class Camera
{
public:
	Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 lookVector, float speed, float fov, float clipNear, float clipFar);

	glm::mat4 UpdateCamera();
	glm::mat4 GetViewMat() const { return m_Projection * m_Matrix; }

	void SetFOV(float fov) { m_FOV = fov; m_ShouldUpdateProjection = true; }

	void ResetMousePos();

	void UpdateKeyStates(int key, int action);
private:
	void m_UpdateProjectionMatrix();
	glm::vec2 m_CalculateMouseOffset(); // Calculates the mouse changes used for the camera rotation
	void m_CalculateRotation(glm::vec2 mouseOffset); // Calculates the view matrix rotation using the mouse offset
	void m_CalculateTranslation(); // Calculates the view translation using the key pressed data and the view matrix

	GLFWwindow* m_WindowPointer;

	// View Matrix Stuff

	glm::vec3 m_LookVector;
	glm::vec3 m_RightVector = glm::vec3(0);
	glm::vec3 m_Position;

	glm::mat4 m_Matrix;
	glm::mat4 m_Projection;

	float m_Speed = 0.0f;

	bool m_KeyWPressed = false;
	bool m_KeyAPressed = false;
	bool m_KeySPressed = false;
	bool m_KeyDPressed = false;

	double m_LastMouseX = 0;
	double m_LastMouseY = 0;

	double m_MouseX = 0;
	double m_MouseY = 0;

	// Projection Matrix stuff

	bool m_ShouldUpdateProjection;

	float m_FOV;
	float m_ClipNear;
	float m_ClipFar;

	int m_WindowWidth = 1280;
	int m_WindowHeight = 720;
};