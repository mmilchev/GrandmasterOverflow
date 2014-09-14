#include "GlobalPower.h"

GlobalPower::GlobalPower(int uses)
	: Power(uses)
{
}

void GlobalPower::OnClicked()
{
	TriggerPower();
}

void GlobalPower::TriggerPower()
{
	if (CanUsePower())
	{
		UsePower();

		OnPowerUsed();
	}
}
