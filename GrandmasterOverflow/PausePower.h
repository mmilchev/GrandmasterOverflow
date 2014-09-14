#ifndef PAUSE_POWER_H
#define PAUSE_POWER_H

#include "GlobalPower.h"

class PausePower
	: public GlobalPower
{
public:
	PausePower(int uses);

protected:
	virtual void UsePower();
	virtual bool CanUsePower() override;
};

#endif