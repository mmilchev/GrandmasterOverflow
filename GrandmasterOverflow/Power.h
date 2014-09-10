#ifndef GUI_BLOCK_H
#define GUI_BLOCK_H

#include <SFML/Window.hpp>
#include <DynamicBehaviour.h>
#include <sigslot.h>
#include <Tween.h>

class Power
	: public DynamicBehaviour
{
public:
	explicit Power(int uses);

	void OnMouseEnter() override;
	void OnMouseLeave() override;

	void Update() override;

protected:
	void OnPowerUsed();
	bool CanUsePower();

	int m_Uses;
	Tween m_ScaleTween;
};

#endif
