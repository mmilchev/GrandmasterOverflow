#ifndef TIME_CONTROL_H
#define TIME_CONTROL_H

#include <DynamicBehaviour.h>

class PauseButtonBehaviour;
class NormalSpeedButtonBehaviour;
class FastforwardButtonBehaviour;

class TimeControl
	: public DynamicBehaviour
{
public:

	void Awake() override;

	void Update() override;

private:
	PauseButtonBehaviour* m_PauseButton;
	NormalSpeedButtonBehaviour* m_NormalSpeedButton;
	FastforwardButtonBehaviour* m_FastForwardButton;
};

#endif