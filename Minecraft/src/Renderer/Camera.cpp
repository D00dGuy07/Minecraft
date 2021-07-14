#include "Camera.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

#include "TimeUtil.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>


Camera::Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 lookVector, float speed, float fov, float clipNear, float clipFar)
	: m_WindowPointer(window),
	m_LookVector(lookVector),
	m_Position(position),
	m_Matrix(glm::lookAt( // Initially the view matrix so that it is displayed correctly at first
		position,
		position + lookVector,
		glm::vec3(0.0f, 1.0f, 0.0f)
	)),
	m_Speed(speed),
	m_FOV(fov), // Perspective matrix stuff
	m_ClipNear(clipNear),
	m_ClipFar(clipFar)
{
	glfwGetFramebufferSize(window, &m_WindowWidth, &m_WindowHeight);
	m_Projection = glm::perspective(glm::radians(m_FOV), (float)m_WindowWidth / (float)m_WindowHeight, m_ClipNear, m_ClipFar);
}

glm::mat4 Camera::UpdateCamera()
{
	m_UpdateProjectionMatrix();

	if (glfwGetInputMode(m_WindowPointer, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
		return m_Projection * m_Matrix;

	glm::vec2 mouseOffset = m_CalculateMouseOffset();
	m_CalculateRotation(mouseOffset);
	m_CalculateTranslation();

	return m_Projection * m_Matrix;
}

void Camera::m_UpdateProjectionMatrix()
{
	int windowWidth = m_WindowWidth;
	int windowHeight = m_WindowHeight;
	glfwGetFramebufferSize(m_WindowPointer, &windowWidth, &windowHeight);

	if (windowWidth != m_WindowWidth || windowHeight != m_WindowHeight)
	{
		std::cout << "The window size was changed!\n";
		m_WindowWidth = windowWidth;
		m_WindowHeight = windowHeight;
		float aspectRatio = (float)m_WindowWidth / (float)m_WindowHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_ClipNear, m_ClipFar);
		glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	}
	else if (m_ShouldUpdateProjection)
	{
		float aspectRatio = (float)m_WindowWidth / (float)m_WindowHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_ClipNear, m_ClipFar);
	}
}

glm::vec2 Camera::m_CalculateMouseOffset()
{
	m_LastMouseX = m_MouseX;
	m_LastMouseY = m_MouseY;
	glfwGetCursorPos(m_WindowPointer, &m_MouseX, &m_MouseY);

	double mouseOffsetX = m_MouseX - m_LastMouseX;
	double mouseOffsetY = m_MouseY - m_LastMouseY;

	return glm::vec2((float)mouseOffsetX, (float)mouseOffsetY);
}

void Camera::m_CalculateRotation(glm::vec2 mouseOffset)
{
	glm::mat4 rotationMat(1); // Creates an identity matrix

	rotationMat = glm::rotate(rotationMat, glm::radians((float)-mouseOffset.x / 2), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_RightVector = glm::normalize(glm::cross(up, m_LookVector));

	rotationMat = glm::rotate(rotationMat, glm::radians((float)mouseOffset.y / 2), m_RightVector);

	m_LookVector = glm::vec3(rotationMat * glm::vec4(m_LookVector, 1.0f));

	glm::vec3 lookPosition = m_Position + m_LookVector;

	glm::vec3 cameraUp = glm::cross(m_LookVector, m_RightVector);

	m_Matrix = glm::lookAt(
		m_Position,
		lookPosition,
		up
	);
}

void Camera::m_CalculateTranslation()
{
	glm::vec3 finalTranslation = glm::vec3(0, 0, 0);

	if (m_KeyWPressed)
	{
		glm::vec3 translation = glm::vec3(
			m_LookVector.x * Time::DeltaTime * m_Speed,
			m_LookVector.y * Time::DeltaTime * m_Speed,
			m_LookVector.z * Time::DeltaTime * m_Speed
		);
		finalTranslation += translation;
		m_Position += translation;
	}
	if (m_KeySPressed)
	{
		glm::vec3 translation = glm::vec3(
			m_LookVector.x * Time::DeltaTime * m_Speed,
			m_LookVector.y * Time::DeltaTime * m_Speed,
			m_LookVector.z * Time::DeltaTime * m_Speed
		);
		finalTranslation += translation;
		m_Position -= translation;
	}
	if (m_KeyAPressed)
	{
		glm::vec3 translation = glm::vec3(
			m_RightVector.x * Time::DeltaTime * m_Speed,
			m_RightVector.y * Time::DeltaTime * m_Speed,
			m_RightVector.z * Time::DeltaTime * m_Speed
		);
		finalTranslation += translation;
		m_Position += translation;
	}
	if (m_KeyDPressed)
	{
		glm::vec3 translation = glm::vec3(
			m_RightVector.x * Time::DeltaTime * m_Speed,
			m_RightVector.y * Time::DeltaTime * m_Speed,
			m_RightVector.z * Time::DeltaTime * m_Speed
		);
		finalTranslation += translation;
		m_Position -= translation;
	}

	glm::translate(m_Matrix, finalTranslation);
}

void Camera::ResetMousePos()
{
	glfwGetCursorPos(m_WindowPointer, &m_MouseX, &m_MouseY);

	m_LastMouseX = m_MouseX;
	m_LastMouseY = m_MouseY;
}

void Camera::UpdateKeyStates(int key, int action)
{
	switch (key)
	{
		case GLFW_KEY_W:
		{
			if (action == GLFW_PRESS)
				m_KeyWPressed = true;
			else if (action == GLFW_RELEASE)
				m_KeyWPressed = false;
			return;
		}
		case GLFW_KEY_A:
		{
			if (action == GLFW_PRESS)
				m_KeyAPressed = true;
			else if (action == GLFW_RELEASE)
				m_KeyAPressed = false;
			return;
		}
		case GLFW_KEY_S:
		{
			if (action == GLFW_PRESS)
				m_KeySPressed = true;
			else if (action == GLFW_RELEASE)
				m_KeySPressed = false;
			return;
		}
		case GLFW_KEY_D:
		{
			if (action == GLFW_PRESS)
				m_KeyDPressed = true;
			else if (action == GLFW_RELEASE)
				m_KeyDPressed = false;
			return;
		}
	}
}
