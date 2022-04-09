#include "AnimatedGraphic.h"
#include <SDL.h>
#include <memory>

AnimatedGraphic::AnimatedGraphic() 
	: ShooterObject()
{

}

void AnimatedGraphic::draw()
{
	ShooterObject::draw();
}

void AnimatedGraphic::update()
{
	m_currentFrame = int(((SDL_GetTicks() / (1000 / m_animSpeed)) %
		2));
}

void AnimatedGraphic::clean()
{
}

void AnimatedGraphic::load(std::unique_ptr<LoaderParams> const &pParams)
{
	ShooterObject::load(pParams);
	m_animSpeed = pParams->getAnimSpeed();
}