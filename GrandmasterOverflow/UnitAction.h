#ifndef UNIT_ACTION_H
#define UNIT_ACTION_H
#include <SFML/System/Vector2.hpp>

class GameObject;

class UnitAction
{
public:
	virtual ~UnitAction() {}

	virtual void Update() { }
	virtual bool Done() const = 0;
protected:
	explicit UnitAction(GameObject* gObject);

	GameObject* m_GameObject;
};

class MoveAction
	: public UnitAction
{

public:
	MoveAction(GameObject* gObject, sf::Vector2f const& destination);

	void Update() override;
	bool Done() const override;
private:
	float m_Time;
	sf::Vector2f m_Origin;
	sf::Vector2f m_Destination;
};

class RotateAction
	: public UnitAction
{
public:
	RotateAction(GameObject* gObject, float target);

	void Update() override;
	bool Done() const override;
private:
	float m_Time;
	float m_Origin;
	float m_Target;
};

class ShootAction
	: public UnitAction
{
public:
	bool Done() const override;

	ShootAction(GameObject* gObject, sf::Vector2f const& dir);

};
#endif