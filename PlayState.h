#pragma once
#include "GameState.h"
#include <vector>
#include "ShooterObject.h"
#include "GameObject.h"
#include "Level.h"

class PlayState 
	: public GameState
{
public:
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();
	virtual std::string getStateID() const { return s_playID; }
	bool checkCollision(ShooterObject* p1, ShooterObject* p2);
private:
	Level* pLevel;
	static const std::string s_playID;
	std::vector<GameObject*> m_gameObjects;
};