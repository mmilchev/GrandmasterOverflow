#ifndef IOBJECTIVE_CHOOSER_H
#define IOBJECTIVE_CHOOSER_H

#include <SFML/System/Vector2.hpp>

class TeamState;

class IObjectiveChooser
{
public:
	virtual ~IObjectiveChooser() {}

public:
	virtual sf::Vector2f GetClosestObjective(TeamState const& teamState, sf::Vector2f const& pos) = 0;
};

class DeathmatchChooser
	:public IObjectiveChooser
{
public:
	sf::Vector2f GetClosestObjective(TeamState const& teamState, sf::Vector2f const& pos) override;

};

class ReachGoalChooser
	:public IObjectiveChooser
{
public:
	sf::Vector2f GetClosestObjective(TeamState const& teamState, sf::Vector2f const& pos) override;
};
#endif