#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include <DynamicBehaviour.h>

class CameraComponent;

class CameraControl
	: public DynamicBehaviour
{
public:
	void Start() override;

	void Update() override;

private:
	void SetSpawnTimer();

	CameraComponent* m_Camera;
	float m_BgEffectSpawnTimer;
};

#endif
