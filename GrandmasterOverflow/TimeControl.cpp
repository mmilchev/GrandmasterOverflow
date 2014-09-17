#include "TimeControl.h"
#include "ButtonBehaviour.h"
#include "Constants.h"
#include "Input.h"
#include "GameTime.h"
#include "LevelManager.h"

#include <GameObject.h>

void TimeControl::Awake()
{
	m_PauseButton = GameObject::FindByTag(TAG_PAUSE_BUTTON)->GetComponent<PauseButtonBehaviour>();
	m_NormalSpeedButton = GameObject::FindByTag(TAG_NORMAL_SPEED_BUTTON)->GetComponent<NormalSpeedButtonBehaviour>();
	m_FastForwardButton = GameObject::FindByTag(TAG_FAST_FORWARD_BUTTON)->GetComponent<FastforwardButtonBehaviour>();
	
	//Pause the game
	m_PauseButton->TriggerButton();
}

void TimeControl::Update()
{
	if (Input::GetKeyDown(sf::Keyboard::Space))
	{
		if (GameTime::GetScale() < 0.1)
			m_NormalSpeedButton->TriggerButton();
		else
			m_PauseButton->TriggerButton();
	}
	else if (Input::GetKeyDown(sf::Keyboard::LShift))
	{
		if (GameTime::GetScale() > 1.01)
			m_NormalSpeedButton->TriggerButton();
		else
			m_FastForwardButton->TriggerButton();
	}
	else if (Input::GetKeyDown(sf::Keyboard::R))
	{
		LevelManager::ReloadCurrentLevel();
	}
}
