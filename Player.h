#pragma once
#include "GameObject.h"
#include "ShooterObject.h"
#include "GameObjectFactory.h"

class Player
	: public ShooterObject 
{
public:

	Player();

	virtual void draw();

	virtual void update();

	virtual void clean();

	virtual void collision();

	virtual void load(std::unique_ptr<LoaderParams> const& pParams);


private:
	// bring the player back if there are lives left
	void ressurect();

	// handle any input from the keyboard, mouse, or joystick
	void handleInput();

	// handle any animation for the player
	void handleAnimation();

	// player can be invulnerable for a time
	int m_invulnerable;
	int m_invulnerableTime;
	int m_invulnerableCounter;

};


class PlayerCreator 
	: public BaseCreator
{
	GameObject* createGameObject() const
	{
		return new Player();
	}
};