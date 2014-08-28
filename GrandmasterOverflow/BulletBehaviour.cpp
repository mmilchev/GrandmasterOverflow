#include "BulletBehaviour.h"
#include "CollisionData.h"
#include "BoardUnit.h"
#include "Constants.h"

#include <GameObject.h>

BulletBehaviour::BulletBehaviour(GameObject* sender)
:m_Sender(sender)
{
}

void BulletBehaviour::OnCollisionEnter(CollisionData& collision)
{
	if (collision.m_GameObject == m_Sender || collision.m_GameObject->GetTag() == TAG_BULLET)
		return;

	GameObject::Destroy(m_GameObject);
}