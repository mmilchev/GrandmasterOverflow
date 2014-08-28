#ifndef SIDEBAR_BEHAVIOUR_H
#define SIDEBAR_BEHAVIOUR_H

#include <DynamicBehaviour.h>
#include <map>
#include <sigslot.h>
#include <SFML/System/Vector2.hpp>

class SidebarBahaviour
	: public DynamicBehaviour, public sigslot::has_slots<>
{
public:
	SidebarBahaviour();

	void OnMouseEnter() override;
	void OnMouseLeave() override;

	void SetupParams();
		
	void Awake() override;
	void Update() override;

	void OnTurnTime();

	inline int GetActivePowerNum() const { return m_ActivePowerNum; }
	inline float GetPowerSquareSize() const { return m_PowerSquareSize; }
	inline float GetBorder() const { return m_Border; }
private:
	void FillEmptyPowers();
	
	std::map<int, watch_ptr<GameObject>> m_AvailablePowers;

	sf::Vector2f m_TargetPos;

	int	m_ActivePowerNum;
	float m_PowerSquareSize;
	float m_Border;
	float m_Width;
	float m_LeftBorder;

	int	m_CurrentTurn;
	int	m_TurnsBeforeBatch;
};

#endif