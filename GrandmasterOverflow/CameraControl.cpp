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
	float speed = ConfigManager::GetFloat("[Camera]fCameraMoveSpeed");
	sf::Vector2f dir = sf::Vector2f(0, 0);
	if (Input::GetKey(sf::Keyboard::W))
		dir.y += -1;
	if (Input::GetKey(sf::Keyboard::S))
		dir.y += 1;
	if (Input::GetKey(sf::Keyboard::A))
		dir.x += -1;
	if (Input::GetKey(sf::Keyboard::D))
		dir.x += 1;

	m_GameObject->Transform()->Translate(dir * speed * GameTime::DeltaTime());

	if (Input::GetKeyDown(sf::Keyboard::Z))
		m_Camera->Zoom(0.9f);
	else if (Input::GetKeyDown(sf::Keyboard::X))
		m_Camera->Zoom(1.1f);
}