#ifndef TILE_DESTROYER_H
#define TILE_DESTROYER_H

#include <DynamicBehaviour.h>

class GameObject;

class TileDestroyer
	: public DynamicBehaviour
{
public:
	void Awake() override;
	void Update() override;

private:
};

#endif
