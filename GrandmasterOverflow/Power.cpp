#include "Power.h"
#include "Constants.h"

#include <GameObject.h>
#include <TransformComponent.h>
#include <GameTime.h>
#include <SpriteRenderer.h>
#include <TextRenderer.h>

#include <iostream>

Power::Power(int uses)
	:m_Uses(uses)
{
}

void Power::OnPowerUsed()
{
	m_Uses--;
	m_GameObject->GetChildWithName(NAME_TEXT_POWER)->GetComponent<TextRenderer>()->Text().setString("x" + std::to_string(m_Uses));
}

bool Power::CanUsePower()
{
	return m_Uses > 0;
}
