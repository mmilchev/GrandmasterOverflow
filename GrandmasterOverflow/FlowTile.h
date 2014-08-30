#ifndef FLOW_TILE_H
#define FLOW_TILE_H

#include <DynamicBehaviour.h>
#include "ITurnClient.h"

class GameState;
class BoardMap;

class FlowTile
	: public DynamicBehaviour, public ITurnClient
{
public:
	explicit FlowTile(int group);

	void OnDestruction() override;

	void Start() override;
	void Update() override;

	void OnTurnTime() override;

	void DestroySurroundings();

	inline int GetFlowGroup() const { return m_Group; }
	inline void SetFlowGroup(int newGroup) { m_Group = newGroup; }
private:
	void ResetTurns();
	void Spread();

	BoardMap* m_Board;
	GameState* m_GameState;

	int	m_TurnsLeft;
	int m_Group;
	bool m_ShouldSpread;
};

#endif
