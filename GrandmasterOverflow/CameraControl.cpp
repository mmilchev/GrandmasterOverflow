#include "CameraControl.h"
#include "Constants.h"
#include "BoardMap.h"

#include <Input.h>
#include <ConfigManager.h>
#include <GameObject.h>
#include <TransformComponent.h>
#include <CameraComponent.h>
#include <GameTime.h>

void CameraControl::Start()
{
	m_Camera = m_GameObject->GetComponent<CameraComponent>();

	auto board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	auto coef = ConfigManager::GetFloat("[Camera]fCameraZoomCoef");
	m_Camera->Zoom(std::max(board->GetWidth(), board->GetHeight()) * coef);
}

void CameraControl::Update()
{
	if (Input::GetKeyDown(sf::Keyboard::Z))
		m_Camera->Zoom(0.9f);
	else if (Input::GetKeyDown(sf::Keyboard::X))
		m_Camera->Zoom(1.1f);
}