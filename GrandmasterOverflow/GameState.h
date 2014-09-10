#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <DynamicBehaviour.h>
#include <sigslot.h>
#include <vector>


class SidebarBahaviour;
class ITurnClient;
class FlowTile;
class TargetPower;

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

	void SolidifyTileGroup(int group);
	
	void OnPowerSelected(TargetPower* power);

private:
	void GetTileGroups(std::vector<int>& groups);
	void TriggerGameOver();

	void CheckTilesForCollisions();
	void ExecuteTurn();

	std::vector<int> m_GroupMovedThisTurn;

	std::vector<FlowTile*> m_FlowTiles;
	std::vector<TargetPower*> m_TargetPowers;

	int	m_TurnNum;
	float m_TurnTime;
	float m_TimeToNextTurn;
};

#endif
