#ifndef GUI_BLOCK_H
#define GUI_BLOCK_H

#include <SFML/Window.hpp>
#include <DynamicBehaviour.h>
#include <sigslot.h>

class Power
	: public DynamicBehaviour
{
public:
	explicit Power(sf::Vector2f const& targetPos);

	void OnMouseEnter() override;
	void OnMouseLeave() override;

	void Update() override;
private:
	sf::Vector2f m_TargetPos;
};

#endif
