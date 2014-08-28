#include "TileState.h"
#include "GameObject.h"
#include <SpriteRenderer.h>

std::map<TileType, std::string> TileState::sTextureTile = 
{ 
	{ Empty, "emptyGridTile.png" }, 
	{ Solid , "solidGridTile.png" } 
};

TileState::TileState()
:m_Occupant(nullptr)
{
}

void TileState::Awake()
{
	m_GameObject->GetComponent<SpriteRenderer>()->SetTextureByName(sTextureTile[m_TileType]);
}

void TileState::SetTileType(TileType type)
{
	m_TileType = type;
	if (m_GameObject != nullptr)
		m_GameObject->GetComponent<SpriteRenderer>()->SetTextureByName(sTextureTile[m_TileType]);
}