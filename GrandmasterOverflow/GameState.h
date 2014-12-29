#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "QueuedVector.h"
#include "FlowTile.h"

#include <DynamicBehaviour.h>
#include <vector>


class SidebarBahaviour;
class ITurnClient;
class TargetPower;
class BoardMap;
class SoundComponent;

class GameState
	: public DynamicBehaviour
{
public:
	GameState();

	void Start() override;
	void Update() override;

	void SetTurnTime(float turnTime);

	void ReportFlowTileCreated(FlowTile* tile);
	void ReportFlowTileDestroyed(FlowTile* tile);

	void ReportPowerCreated(TargetPower* power);

	void ReportTileActivity(FlowTile* tile);

	void SolidifyTileType(FlowTile::FlowTileType type);
	
	void OnPowerSelected(TargetPower* power);

private:
	void GetTileTypes(std::vector<FlowTile::FlowTileType>& types);
	void TriggerGameOver();

	void CheckAndSolidifyTiles();
	void ExecuteTurn();

	std::vector<FlowTile::FlowTileType> m_TypesMovedThisTurn;

	BoardMap* m_Board;
	SoundComponent* m_SoundComp;

	QueuedVector<FlowTile*> m_FlowTiles;
	std::vector<TargetPower*> m_TargetPowers;

	int	m_TurnNum;
	float m_TurnTime;
	float m_TimeToNextTurn;

	bool m_GameOver;
};

#endif
