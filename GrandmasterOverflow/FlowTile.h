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
		LightBlue
	};
	FlowTile(FlowTileType type, int group, int turns);

	void OnDestruction() override;

	void Start() override;
	void Update() override;

	void OnTurnTime() override;

	FlowTile const* CheckCollision();

	void Solidify();

	bool IsOutOfMoves() const;

	inline int GetFlowGroup() const { return m_Group; }
	inline void SetFlowGroup(int newGroup) { m_Group = newGroup; }
	inline void SetShouldScale(bool shouldScale) { m_ShoudScale = shouldScale; }

private:
	void ResetTurns();
	void Spread();

	BoardMap* m_Board;
	GameState* m_GameState;

	FlowTileType const kType;

	int m_SpreadTurnsLeft;

	int	m_TurnsLeft;
	int m_Group;
	bool m_ShouldSpread;
	bool m_ShoudScale;

	Tween2 m_ScaleTween;

	static std::map<FlowTileType, sf::Color> sTileColours;
	static const std::vector<sf::Vector2i> skSpreadDirections;
};

#endif
