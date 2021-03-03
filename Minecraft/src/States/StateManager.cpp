#include "StateManager.h"

#include <iostream>

#include "PlayingState.h"

StateManager::StateManager(GLFWwindow* window)
	: m_WindowPointer(window)
{
	// Initialize State Objects
	m_States["PlayingState"] = new PlayingState;

	// Load "main" state
	m_LoadedState = "PlayingState";
	m_CurrentState = m_States[m_LoadedState];
	m_CurrentState->load(m_WindowPointer);
}

void StateManager::Update()
{
	if (m_LoadedState == "")
		return;

	GameStateMessage loopResults = m_CurrentState->loop(m_WindowPointer);

	switch (loopResults.Status) 
	{
	case GameStateStatus::OKAY:
		break;
	case GameStateStatus::EXIT:
		m_CurrentState->close(m_WindowPointer);
		m_CurrentState = nullptr;
		m_LoadedState = "";
	case GameStateStatus::CRASH:
		m_CurrentState->close(m_WindowPointer);
		std::cout << "[Game State Manager]: " << m_LoadedState << " has crashed with the message: " << loopResults.Message << std::endl;
		m_CurrentState = nullptr;
		m_LoadedState = "";
	}
}

StateManager::~StateManager()
{
	if (m_LoadedState != "")
		m_States[m_LoadedState]->close(m_WindowPointer);

	for (const auto& element : m_States) {
		delete element.second;
	}
}
