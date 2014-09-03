#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <DynamicBehaviour.h>
#include <sigslot.h>
#include <vector>


class SidebarBahaviour;
class ITurnClient;
class FlowTile;

class GameState
	: public DynamicBehaviour
{
public:
	GameState();

	void Start() override;
	void Update() override;

	void SetTurnTime(float turnTime);

	void ConnectClient(ITurnClient* client);
	void DisconnectClient(ITurnClient* client);

	void ReportFlowTileCreated(FlowTile* tile);
	void ReportFlowTileDestroyed(FlowTile* tile);
	void MergeFlowGroups(int groupLhs, int groupRhs);

	void ReportTileSpread(FlowTile* tile);

private:
	void GetTileGroups(std::vector<int>& groups);
	void Solidify(int group);
	void TriggerGameOver();

	sigslot::signal0<> m_TurnTimeSignal;

	std::vector<int> m_GroupMovedThisTurn;

	std::vector<FlowTile*> m_FlowTiles;

	int	m_TurnNum;
	float m_TurnTime;
	float m_TimeToNextTurn;
};

#endif
