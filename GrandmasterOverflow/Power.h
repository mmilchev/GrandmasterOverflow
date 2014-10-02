#ifndef GUI_BLOCK_H
#define GUI_BLOCK_H

#include <SFML/Window.hpp>
#include <DynamicBehaviour.h>
#include <Tween.h>

class Power
	: public DynamicBehaviour
{
public:
	explicit Power(int uses);

protected:
	void OnPowerUsed();
	virtual bool CanUsePower();

	int m_Uses;
};

#endif
