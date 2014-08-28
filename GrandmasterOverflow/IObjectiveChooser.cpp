#include "IObjectiveChooser.h"
#include "TeamState.h"
#include <algorithm>
#include <Utils.h>

sf::Vector2f DeathmatchChooser::GetClosestObjective(TeamState const& teamState, sf::Vector2f const& pos)
{
	auto& spots = teamState.GetSeenSpots();
	if (spots.size() > 0)
	{
		//Note: This can be cached to increase performance
		std::vector<sf::Vector2f> positions;
		for (auto& spot : spots)
			positions.push_back(spot.m_Spot);
		return *std::min_element(positions.begin(), positions.end(), 
			[&pos](sf::Vector2f const& lhs, sf::Vector2f const& rhs){ return Length(lhs - pos) < Length(rhs - pos); });
	}
	else
	{
		auto& objectives = teamState.GetObjectiveSpots();
		
		//If no objectives
		if (objectives.size() == 0)
			return sf::Vector2f(0, 0);

		return *std::min_element(objectives.begin(), objectives.end(),
			[&pos](sf::Vector2f const& lhs, sf::Vector2f const& rhs){ return Length(lhs - pos) < Length(rhs - pos); });
	}
}

sf::Vector2f ReachGoalChooser::GetClosestObjective(TeamState const& teamState, sf::Vector2f const& pos)
{
	auto& objectives = teamState.GetObjectiveSpots();
	return *std::min_element(objectives.begin(), objectives.end(),
		[&pos](sf::Vector2f const& lhs, sf::Vector2f const& rhs){ return Length(lhs - pos) < Length(rhs - pos); });
}