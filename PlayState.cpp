#include "PlayState.h"
#include <iostream>
#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include "PauseState.h"
#include "InputHandler.h"
#include "Enemy.h"
#include "GameOverState.h"
#include "LevelParser.h"
#include "BulletHandler.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
	if (m_loadingComplete && !m_exiting)
	{
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
		{
			TheGame::Instance()->getStateMachine()->pushState(new PauseState());
		}

		//        if(TheInputHandler::Instance()->getButtonState(0, 8))
		//        {
		//            TheGame::Instance()->getStateMachine()->pushState(new PauseState());
		//        }

		TheBulletHandler::Instance()->updateBullets();

		if (TheGame::Instance()->getPlayerLives() == 0)
		{
			
			TheGame::Instance()->getStateMachine()->
				changeState(new GameOverState());

		}

		if (pLevel != 0)
		{
			pLevel->update();
		}
	}
}

void PlayState::render()
{
	if (m_loadingComplete)
	{
		if (pLevel != 0)
		{
			pLevel->render();
		}

		for (int i = 0; i < TheGame::Instance()->getPlayerLives(); i++)
		{
			TheTextureManager::Instance()->drawFrame("lives", i * 30, 0, 32, 
				30, 0, 0, TheGame::Instance()->getRenderer(), 0.0, 255);
		}

		TheBulletHandler::Instance()->drawBullets();
	}
}

bool PlayState::onEnter()
{
	LevelParser levelParser;

	pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel() - 1].c_str());
	TheTextureManager::Instance()->load("assets/bullet1.png", "bullet1", TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->load("assets/bullet2.png", "bullet2", TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->load("assets/bullet3.png", "bullet3", TheGame::Instance()->getRenderer());
	TheTextureManager::Instance()->load("assets/lives.png", "lives", TheGame::Instance()->getRenderer());

	if (pLevel != 0)
	{
		
		m_loadingComplete = true;
	}

	std::cout << "entering PlayState\n";
	return true;
}

bool PlayState::onExit()
{
	m_exiting = true;

	TheInputHandler::Instance()->reset();
	TheBulletHandler::Instance()->clearBullets();

	std::cout << "exiting PlayState\n";
	return true;
}

bool PlayState::checkCollision(ShooterObject* p1, ShooterObject* p2)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	leftA = p1->getPosition().getX();
	rightA = p1->getPosition().getX() + p1->getWidth();
	topA = p1->getPosition().getY();
	bottomA = p1->getPosition().getY() + p1->getHeight();
	//Calculate the sides of rect B
	leftB = p2->getPosition().getX();
	rightB = p2->getPosition().getX() + p2->getWidth();
	topB = p2->getPosition().getY();
	bottomB = p2->getPosition().getY() + p2->getHeight();
	//If any of the sides from A are outside of B
	if (bottomA <= topB) { return false; }
	if (topA >= bottomB) { return false; }
	if (rightA <= leftB) { return false; }
	if (leftA >= rightB) { return false; }
	return true;
}

