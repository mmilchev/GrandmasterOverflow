#include "CameraControl.h"
#include "Constants.h"
#include "BoardMap.h"

#include <Input.h>
#include <ConfigManager.h>
#include <GameObject.h>
#include <TransformComponent.h>
#include <CameraComponent.h>
#include <GameTime.h>
#include "Utils.h"
#include "FlowTile.h"
#include "Prefabs.h"
#include "Application.h"

void CameraControl::Start()
{
	m_Camera = m_GameObject->GetComponent<CameraComponent>();

	auto board = GameObject::FindByTag(TAG_GAME_BOARD)->GetComponent<BoardMap>();
	auto coef = ConfigManager::GetFloat("[Camera]fCameraZoomCoef");
	m_Camera->Zoom(std::max(board->GetWidth(), board->GetHeight()) * coef);

	SetSpawnTimer();
}

void CameraControl::Update()
{
	if (Input::GetKeyDown(sf::Keyboard::Z))
		m_Camera->Zoom(0.9f);
	else if (Input::GetKeyDown(sf::Keyboard::X))
		m_Camera->Zoom(1.1f);

	m_BgEffectSpawnTimer -= GameTime::DeltaTimeUnscaled();
	if (m_BgEffectSpawnTimer <= 0)
	{
		auto& dir = FlowTile::skSpreadDirections[RandInt(0, FlowTile::skSpreadDirections.size())];
		auto wSize = static_cast<sf::Vector2f>(Application::GetWindow().getSize());
		float wFactor = dir.x * wSize.x; float hFactor = dir.y * wSize.y;
		auto pixel = sf::Vector2f(
			Clamp(-wFactor, 0, static_cast<float>(wSize.x)) + std::abs(dir.y) * Randf(0.25f * wSize.x, 0.75f * wSize.x),
			Clamp(-hFactor, 0, static_cast<float>(wSize.y)) + std::abs(dir.x) * Randf(0.25f * wSize.y, 0.75f * wSize.y)
			);
		auto pos = Application::GetWindow().mapPixelToCoords(static_cast<sf::Vector2i>(pixel), m_Camera->GetView());
		int power = RandInt(ConfigManager::GetInt("[Bg Effect]iMinPower"), ConfigManager::GetInt("[Bg Effect]iMaxPower"));
		auto& color = FlowTile::sTileColours[static_cast<FlowTile::FlowTileType>(RandInt(0, FlowTile::Count))];
		GameObject::Instantiate(
			prefabs::CreateBgEffectTile(pos, dir, color, power, ConfigManager::GetFloat("[Bg Effect]fBranchStartChance")));
		SetSpawnTimer();
	}
}

void CameraControl::SetSpawnTimer()
{
	float delta = ConfigManager::GetFloat("[Bg Effect]fSpawnPeriodDelta");
	m_BgEffectSpawnTimer = ConfigManager::GetFloat("[Bg Effect]fSpawnPeriod") + Randf(-delta, delta);
}
