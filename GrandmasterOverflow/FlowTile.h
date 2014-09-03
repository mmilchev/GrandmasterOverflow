#ifndef FLOW_TILE_H
#define FLOW_TILE_H

#include <DynamicBehaviour.h>
#include "ITurnClient.h"
#include <SFML/Graphics/Color.hpp>
#include <map>

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
	FlowTile(FlowTileType type, int group);

	void OnDestruction() override;

	void Start() override;
	void Update() override;

	void OnTurnTime() override;

	void Solidify();

	inline int GetFlowGroup() const { return m_Group; }
	inline void SetFlowGroup(int newGroup) { m_Group = newGroup; }
private:
	void ResetTurns();
	void Spread();

	BoardMap* m_Board;
	GameState* m_GameState;

	FlowTileType const kType;

	int	m_TurnsLeft;
	int m_Group;
	bool m_ShouldSpread;

	static std::map<FlowTileType, sf::Color> sTileColours;
};

#endif
