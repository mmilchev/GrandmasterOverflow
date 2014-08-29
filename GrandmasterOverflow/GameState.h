#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <DynamicBehaviour.h>
#include <sigslot.h>


class SidebarBahaviour;
class ITurnClient;

class GameState
	: public DynamicBehaviour
{
public:
	GameState();

	void Start() override;
	void Update() override;
	
	void SetTurnTime(float turnTime);

	void ConnectClient(ITurnClient* client);
	void DisconnectClient(ITurnClient* client);

private:
	sigslot::signal0<> m_TurnTimeSignal;

	float m_TurnTime;
	float m_TimeToNextTurn;
};

#endif
