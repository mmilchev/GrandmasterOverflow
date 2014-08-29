#ifndef ITURN_CLIENT_H
#define ITURN_CLIENT_H

#include <sigslot.h>

class ITurnClient
	: public sigslot::has_slots<>
{
public:
	virtual void OnTurnTime() = 0;
};

#endif