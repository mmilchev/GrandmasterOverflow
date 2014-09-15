#ifndef SCREEN_ICON_ANIMATION_H
#define SCREEN_ICON_ANIMATION_H

#include <DynamicBehaviour.h>
#include <Tween.h>

class SpriteRenderer;

class ScreenIconAnimation
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
