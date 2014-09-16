#ifndef SCREEN_SCALE_ANIMATION_H
#define SCREEN_SCALE_ANIMATION_H

#include <DynamicBehaviour.h>
#include <Tween.h>

class SpriteRenderer;

class ScreenScaleAnimation
	: public DynamicBehaviour
{
public:
	void Awake() override;

	void Update() override;
	
private:
	Tween m_FadeTween;
	SpriteRenderer* m_Renderer;
};

#endif // !PAUSE_INDICATOR_H
