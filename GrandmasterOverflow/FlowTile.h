#ifndef FLOW_TILE_H
#define FLOW_TILE_H

#include <DynamicBehaviour.h>
#include "ITurnClient.h"

class BoardMap;

class FlowTile
	: public DynamicBehaviour, public ITurnClient
{
public:
	FlowTile();

	void OnDestruction() override;

	void Start() override;
	void Update() override;

	void OnTurnTime() override;

private:
	void ResetTurns();
	void Spread();

	BoardMap* m_Board;
	int	m_TurnsLeft;
	bool m_ShouldSpread;
};

#endif
