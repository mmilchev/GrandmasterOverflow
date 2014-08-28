#ifndef TEAM_STATE_H
#define TEAM_STATE_H
#include <DynamicBehaviour.h>
#include "GameState.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>
#include <map>

class IObjectiveChooser;

class TeamState
	:public DynamicBehaviour
{
public:
	struct UnitSpot
	{
		explicit UnitSpot(GameObject* obj);

		watch_ptr<GameObject> m_Unit;
		sf::Vector2f m_Spot;
	};
	explicit TeamState(int teamNum);

	void Awake() override;
	void Update() override;

	sf::Vector2f GetClosestObjective(sf::Vector2f const& position) const;

	void OnUnitSpotted(GameObject* gObject);
	void AddObjectiveSpot(sf::Vector2f const& spot);

	void SetGameMode(GameType type);

	inline std::vector<UnitSpot> const& GetSeenSpots() const { return m_SeenSpots; }
	inline std::vector<sf::Vector2f> const& GetObjectiveSpots() const { return m_ObjectiveSpots; }
	inline int GetTeamNum() const { return m_TeamNum; }
private:
	int m_TeamNum;

	std::unique_ptr<IObjectiveChooser> m_ObjectiveChooser;

	std::vector<UnitSpot> m_SeenSpots;
	std::vector<sf::Vector2f> m_ObjectiveSpots;
};

#endif