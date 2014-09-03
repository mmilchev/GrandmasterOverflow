#include "TileState.h"
#include "GameObject.h"
#include <SpriteRenderer.h>

std::map<TileType, std::string> TileState::sTextureTile = 
{ 
	{ Empty, "emptyGridTile.png" }, 
	{ Solid , "solidGridTile.png" },
};

std::map<TileType, sf::Color> TileState::sColorTile =
{
	{ Empty, sf::Color::White },
	{ Solid, sf::Color::White },
};

TileState::TileState()
:m_Occupant(nullptr)
{
}

void TileState::Awake()
{
	SetTileType(m_TileType);
}

void TileState::SetTileType(TileType type)
{
	m_TileType = type;
	if (m_GameObject != nullptr)
	{
		auto renderer = m_GameObject->GetComponent<SpriteRenderer>();
		renderer->SetTextureByName(sTextureTile[m_TileType]);
		renderer->SetSpriteColor(sColorTile[m_TileType]);
	}
		
}