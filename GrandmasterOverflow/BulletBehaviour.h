#ifndef BULLET_BEHAVIOUR_H
#define BULLET_BEHAVIOUR_H

#include <DynamicBehaviour.h>

class BulletBehaviour
	: public DynamicBehaviour 
{
public:
	explicit BulletBehaviour(GameObject* sender);
	void OnCollisionEnter(CollisionData& collision) override;

	inline GameObject const* GetSender() const { return m_Sender; }
private:
	GameObject* m_Sender;
};

#endif
