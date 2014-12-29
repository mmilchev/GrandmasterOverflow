#ifndef BG_EFFECT_TILE_H
#define BG_EFFECT_TILE_H

#include <DynamicBehaviour.h>
#include <SFML/System/Vector2.hpp>

class SpriteRenderer;

class BgEffectTile :
	public DynamicBehaviour
{
public:
	BgEffectTile(sf::Vector2i const& dir, int power, float m_BranchChance);

	virtual void Update();

	virtual void Awake();

private:
	bool ShouldChangeDir() const;
	bool ShouldBranch() const;
	bool ShouldSpread() const;

	sf::Vector2i GetNewDir(sf::Vector2i const& oldDir) const;

	const float m_CellSize;
	const float m_FadeAfter;
	const float m_FadeDuration;
	const float m_SpreadTime;
	
	float m_BranchChance;
	int m_Power;
	sf::Vector2i m_Dir;

	SpriteRenderer* m_Renderer;

	bool m_ShouldSpread;
	float m_Time;
};

#endif // ifndef

