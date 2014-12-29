#include "BgEffectTile.h"
#include "Prefabs.h"
#include <ConfigManager.h>
#include <Application.h>
#include <GameTime.h>
#include <GameObject.h>
#include <TransformComponent.h>
#include <SpriteRenderer.h>
#include "Utils.h"
#include "FlowTile.h"


BgEffectTile::BgEffectTile(sf::Vector2i const& dir, int power, float branchChance)
	: m_CellSize(ConfigManager::GetFloat("[Bg Effect]fCellFactor") * Application::GetWindow().getSize().x),
	m_FadeAfter(ConfigManager::GetFloat("[Bg Effect]fFadeAfter")), m_FadeDuration(ConfigManager::GetFloat("[Bg Effect]fFadeDuration")),
	m_SpreadTime(ConfigManager::GetFloat("[Bg Effect]fSpreadTime")), m_BranchChance(branchChance), m_Dir(dir), m_Power(power), m_ShouldSpread(true), m_Time(0) 
{
}

void BgEffectTile::Awake()
{
	m_Renderer = m_GameObject->GetComponent<SpriteRenderer>();
	m_Renderer->SetSpriteSize(sf::Vector2f(m_CellSize, m_CellSize));
}

void BgEffectTile::Update()
{
	m_Time += GameTime::DeltaTimeUnscaled();
	if (ShouldSpread())
	{
		std::vector<sf::Vector2i> dirs;
		if (ShouldChangeDir())
		{
			m_Dir = GetNewDir(m_Dir);
		}
		else if (ShouldBranch())
		{
			m_BranchChance *= ConfigManager::GetFloat("[Bg Effect]fBranchMultiplier");
			dirs.push_back(GetNewDir(m_Dir));
		}
		dirs.push_back(m_Dir);

		for (auto& dir : dirs)
		{
			GameObject::Instantiate(prefabs::CreateBgEffectTile(m_GameObject->Transform()->Position() + 
				static_cast<sf::Vector2f>(dir)* m_CellSize, m_Dir, m_Renderer->GetSprite().getColor(), 
				(m_Power - 1) / dirs.size(), m_BranchChance));
		}

		m_ShouldSpread = false;
	}
	if (m_Time > m_FadeAfter)
	{
		float coef = (m_Time - m_FadeAfter) / m_FadeDuration;
		if (coef > 1)
		{
			GameObject::Destroy(m_GameObject);
			return;
		}

		auto& sprite = m_Renderer->GetSprite();
		auto color = sprite.getColor();
		color.a = static_cast<sf::Uint8>((1 - coef) * 255);
		sprite.setColor(color);
	}
}

bool BgEffectTile::ShouldChangeDir() const
{
	return Randf(0, 1) < ConfigManager::GetFloat("[Bg Effect]fChangeDirChance");
}

bool BgEffectTile::ShouldBranch() const
{
	return Randf(0, 1) < m_BranchChance;
}

bool BgEffectTile::ShouldSpread() const
{
	float powerCoef = m_Power / static_cast<float>(ConfigManager::GetInt("[Bg Effect]iMaxPower"));
	float spreadTime = m_SpreadTime * pow(powerCoef, ConfigManager::GetFloat("[Bg Effect]fSpreadPower"));
	return m_Time > spreadTime && m_Power > 0 && m_ShouldSpread;
}

sf::Vector2i BgEffectTile::GetNewDir(sf::Vector2i const& oldDir) const
{
	sf::Vector2i newDir;
	do 
	{
		newDir = FlowTile::skSpreadDirections[RandInt(0, 4)];
	} while (oldDir == newDir || Dot(newDir, oldDir) < -0.1);
	return newDir;
}
