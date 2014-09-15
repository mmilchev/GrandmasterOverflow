#ifndef BUTTON_BEHAVIOUR
#define BUTTON_BEHAVIOUR

#include <DynamicBehaviour.h>

#include "Tween.h"

class ButtonBehaviour
	: public DynamicBehaviour
{
public:
	ButtonBehaviour();

	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void OnClicked() override;

	void Update() override;

	virtual void TriggerButton();

private:
	Tween m_ScaleTween;
};

class PauseButtonBehaviour
	: public ButtonBehaviour
{
public:
	void TriggerButton() override;
};

class NormalSpeedButtonBehaviour
	: public ButtonBehaviour
{
public:
	void TriggerButton() override;
};

class FastforwardButtonBehaviour
	: public ButtonBehaviour
{
public:
	void TriggerButton() override;
};

#endif