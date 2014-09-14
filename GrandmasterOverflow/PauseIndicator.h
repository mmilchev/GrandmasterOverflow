#ifndef PAUSE_INDICATOR_H
#define PAUSE_INDICATOR_H

#include <DynamicBehaviour.h>
#include <Tween.h>

class SpriteRenderer;

class PauseIndicator
	: public DynamicBehaviour
{
public:

	void OnClicked() override;
	void Awake() override;

	void Update() override;
	
private:
	Tween m_FadeTween;
	SpriteRenderer* m_Renderer;
};

#endif // !PAUSE_INDICATOR_H
