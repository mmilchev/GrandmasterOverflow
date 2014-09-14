#ifndef GLOBAL_POWER_H
#define GLOBAL_POWER_H

#include "Power.h"

class GlobalPower
	: public Power
{
public:
	explicit GlobalPower(int uses);

	void OnClicked() override;

protected:
	void TriggerPower();

	virtual void UsePower() = 0;
};

#endif