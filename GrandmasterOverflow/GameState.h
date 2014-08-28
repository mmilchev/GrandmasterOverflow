#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <DynamicBehaviour.h>
#include <sigslot.h>

enum GameType
{
	Deathmatch = 0,
	ReachGoal
};

class SidebarBahaviour;
class BoardUnit;

class GameState
	: public DynamicBehaviour
{
public:
	explicit GameState(GameType);

	void Start() override;
	void Update() override;
	
	void SetTurnTime(float turnTime);

	void ConnectUnit(BoardUnit* unit);
	void ConnectSidebar(SidebarBahaviour* sidebar);
	void DisconnectUnit(BoardUnit* uint);

	inline GameType GetGameType() const { return m_GameType; }
private:
	sigslot::signal0<> m_TurnTimeSignal;

	GameType m_GameType;
	float m_TurnTime;
	float m_TimeToNextTurn;
};

#endif
