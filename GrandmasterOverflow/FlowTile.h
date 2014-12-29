#ifndef FLOW_TILE_H
#define FLOW_TILE_H

#include <DynamicBehaviour.h>
#include "ITurnClient.h"
#include <SFML/Graphics/Color.hpp>
#include <map>
#include <Tween2.h>
#include <vector>

class GameState;
class BoardMap;

class FlowTile
	: public DynamicBehaviour, public ITurnClient
{
public:
	enum FlowTileType
	{
		Red,
		Blue,
		Teal,
		Yellow,
		Orange,
		Green,
		Pink,
		Gray,
		LightBlue,
		Count
	};
	FlowTile(FlowTileType type, int turns);

	void Start() override;
	void Update() override;

	void OnTurnTime() override;

	FlowTile const* CheckCollision();

	void Solidify();

	bool IsOutOfMoves() const;

	inline FlowTileType GetTileType() const { return kType; }
	inline void SetShouldScale(bool shouldScale) { m_ShoudScale = shouldScale; }

	static const std::vector<sf::Vector2i> skSpreadDirections;
	static std::map<FlowTileType, sf::Color> sTileColours;
private:
	void ResetTurns();
	void Spread();

	BoardMap* m_Board;
	GameState* m_GameState;

	FlowTileType const kType;

	int m_SpreadTurnsLeft;

	int	m_TurnsLeft;
	bool m_ShouldSpread;
	bool m_ShoudScale;

	Tween2 m_ScaleTween;
};

#endif
