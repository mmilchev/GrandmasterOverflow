#include "PlaceTilePower.h"
#include "BoardMap.h"
#include "GhostPower.h"

#include <SpriteRenderer.h>
#include <GameObject.h>

PlaceTilePower::PlaceTilePower(Type type, int uses)
	: TargetPower(uses), m_Type(type)
{
}

bool PlaceTilePower::CanBePlaced(sf::Vector2i const& pos)
{
	auto tiles = m_GhostPower->GetGameObject()->GetChildrenWithTag(TAG_POWER_TILE);
	for (auto tile : tiles)
	{
		auto gridPos = m_Board->GetGridPos(tile->Transform()->Position());
		if (tile->GetName() == NAME_POWER_TILE_CREATE)
		{
			if (!m_Board->IsInBounds(gridPos) || m_Board->IsOccupied(gridPos)
				|| m_Board->GetTileState(gridPos)->GetTileType() != TileType::Empty)
			{
				return false;
			}
		}
		else if (tile->GetName() == NAME_POWER_TILE_DESTROY)
		{
			if (!m_Board->IsInBounds(gridPos) || m_Board->IsOccupied(gridPos) || m_Board->GetTileState(gridPos)->GetTileType() == TileType::Empty)
				return false;
		}
	}
	return true;
}

void PlaceTilePower::PopulateGhostPower()
{
	m_GhostPower->SetOrigin(m_TileGrid.m_GhostOrigin);

	for (auto& tile : m_TileGrid.m_Tiles)
	{
		auto gGhostObject = m_GhostPower->GetGameObject();

		auto gObject = new GameObject();
		gObject->SetLayer(Layer::Game);
		gObject->SetName((tile.m_Type == Tiles::TileType::Create) ?
		NAME_POWER_TILE_CREATE : NAME_POWER_TILE_DESTROY);
		gObject->SetTag(TAG_POWER_TILE);

		auto renderer = new SpriteRenderer("solidGridTile.png");
		renderer->SetSpriteSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		renderer->SetSpriteColor(sf::Color(170, 170, 170, 255));
		gObject->AddComponent(renderer);

		gObject->SetParent(gGhostObject);

		gObject->Transform()->SetLocalPosition(tile.m_Pos);
	}
}

void PlaceTilePower::UsePower(sf::Vector2i const& pos)
{
	auto tiles = m_GhostPower->GetGameObject()->GetChildrenWithTag(TAG_POWER_TILE);
	for (auto tile : tiles)
	{
		auto gridPos = m_Board->GetGridPos(tile->Transform()->Position());
		if (tile->GetName() == NAME_POWER_TILE_CREATE)
			m_Board->CreateSolidTile(gridPos);
		else if (tile->GetName() == NAME_POWER_TILE_DESTROY)
			m_Board->CreateEmptyTile(gridPos);
	}
}

void PlaceTilePower::Awake()
{
	TargetPower::Awake();
	m_TileGrid.m_GhostOrigin = sf::Vector2f(0, 0);
	switch (m_Type)
	{
	case PlaceTilePower::Single:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(0, 0));
		break;
	case PlaceTilePower::SingleDestroy:
		m_TileGrid.AddTile(Tiles::TileType::Destroy, sf::Vector2f(0, 0));
		break;
	case PlaceTilePower::CornerSmall:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE / 2, -TILE_SIZE / 2));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE / 2, -TILE_SIZE / 2));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE / 2, TILE_SIZE / 2));
		m_TileGrid.m_GhostOrigin = sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2);
		break;
	case PlaceTilePower::CornerBig:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE, -TILE_SIZE));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(0, -TILE_SIZE));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE, -TILE_SIZE));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE, TILE_SIZE));
		break;
	case PlaceTilePower::IShapeSmall:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(0, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE, 0));
		break;
	case PlaceTilePower::IShapeBig:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-2 * TILE_SIZE, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(0, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(2 * TILE_SIZE, 0));
		break;
	case PlaceTilePower::OShape:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE / 2, -TILE_SIZE / 2));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE / 2, -TILE_SIZE / 2));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE / 2, TILE_SIZE / 2));
		m_TileGrid.m_GhostOrigin = sf::Vector2f(TILE_SIZE / 2, TILE_SIZE / 2);
		break;
	case PlaceTilePower::TShape:
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(0, -TILE_SIZE));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(-TILE_SIZE, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(0, 0));
		m_TileGrid.AddTile(Tiles::TileType::Create, sf::Vector2f(TILE_SIZE, 0));
		break;
	default:
		break;
	}
}
