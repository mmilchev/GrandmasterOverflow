#ifndef FLOW_TILE_H
#define FLOW_TILE_H

#include <DynamicBehaviour.h>
#include "ITurnClient.h"

class FlowTile
	: public DynamicBehaviour, public ITurnClient
{
public:
	void OnTurnTime() override;
};

#endif
