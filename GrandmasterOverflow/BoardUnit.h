#ifndef BOARD_UNIT_H
#define BOARD_UNIT_H

#include "UnitAction.h"
#include "GameState.h"

#include <DynamicBehaviour.h>
#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include <vector>

class TeamState;
class BoardMap;

class BoardUnit
	: public DynamicBehaviour, public sigslot::has_slots<>
{
public:
	void Awake() override;
	void Update() override;
	void OnDestruction() override;

	void ExecuteAction(UnitAction* action);
	bool IsReadyForAction() const;

	void OnTurnTime();

	void SetTeam(int teamNum);
	inline int GetTeam() const { return m_TeamNum; }
	inline void SetTurnsPerActions(int turns) { m_TurnsPerAction = turns; }
protected:
	class ClosestUnit
		: public b2RayCastCallback
	{
	public:

		float32 ReportFixture(b2Fixture* fixture, b2Vec2 const& point, b2Vec2 const& normal, float32 fraction) override;

		inline void Reset(int ignoreTeam);

		inline bool HasData();
		inline GameObject* GetGameObject();

		int			m_IgnoreTeam;

		b2Fixture*	m_ClosestFixture;
		sf::Vector2f m_ClosestPoint;
		sf::Vector2f m_ClosestNormal;
	private:

		inline GameObject* GetGameObject(b2Fixture* fixture);
	};

	BoardUnit();

	bool GetClosestUnitSeen(GameObject*& gameObject);
	bool CanSeeUnit(GameObject* gameObject);

	bool IsActionRunning() const;

	TeamState* m_TeamState;
	std::vector<GameObject*> m_RaycastSpots;

private:
	bool CanSeeUnit(GameObject* gameObject, std::vector<std::pair<GameObject*, ClosestUnit>>& raycastData);

	std::unique_ptr<UnitAction> m_CurrentAction;
	GameState* m_GameState;
	BoardMap* m_Map;

	int	m_TurnsPerAction;
	int m_CurrentTurn;

	int	m_TeamNum;
};

#endif