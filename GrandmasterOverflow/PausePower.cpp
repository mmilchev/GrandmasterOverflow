#include "PausePower.h"
#include "Constants.h"
#include "Prefabs.h"

#include <GameObject.h>

PausePower::PausePower(int uses)
	: GlobalPower(uses)
{
}

void PausePower::UsePower()
{
	GameObject::Instantiate(prefabs::CreatePauseHandler());
}

bool PausePower::CanUsePower()
{
	return GlobalPower::CanUsePower() && (GameObject::FindByTag(TAG_PAUSE_HANDLER) == nullptr);
}