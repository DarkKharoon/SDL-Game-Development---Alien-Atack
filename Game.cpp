#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "MainMenuState.h"
#include "GameObjectFactory.h"
#include "MenuButton.h"
#include "AnimatedGraphic.h"
#include "Player.h"
#include "ScrollingBackground.h"
#include "SoundManager.h"
#include "RoofTurret.h"
#include "ShotGlider.h"
#include "Eskeletor.h"
#include "Level1Boss.h"
#include "GameOverState.h"
#include <iostream>
#include "PlayState.h"


Game* Game::s_pInstance = 0;
GameStateMachine* m_pGameStateMachine;

bool Game::init(const char* title, int xpos, int ypos, int widht, int height, int flags) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL Init success\n";
		 //create the window 
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, widht, height, flags);
		if (m_pWindow != 0) //window init success 
		{
			std::cout << "Window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0) // renderer init success
			{
				std::cout << "Renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
			}
			else {
				std::cout << "renderer init fail\n";
				return false;
			}
		}
		else {
			std::cout << "Window init fail\n";
			return false;
		}
	}
	else {
		std::cout << "SDL init fail\n";
		return false;
	}
	m_gameWidth = widht;
	m_gameHeight = height;

	// add some sound effects - TODO move to better place
	//TheSoundManager::Instance()->load("assets/The Weeknd - Take My Breath.ogg", "weeknd", SOUND_MUSIC);
	TheSoundManager::Instance()->load("assets/DST_ElectroRock.ogg", "music1", SOUND_MUSIC);
	TheSoundManager::Instance()->load("assets/boom.wav", "explode", SOUND_SFX);
	TheSoundManager::Instance()->load("assets/phaser.wav", "shoot", SOUND_SFX);

	TheSoundManager::Instance()->playMusic("music1", -1);

	std::cout << "init success\n";

	m_bRunning = true; //everything inited successfully, start the main loop

	// register the types for the game
	TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
	TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
	TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
	TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
	TheGameObjectFactory::Instance()->registerType("Turret", new TurretCreator());
	TheGameObjectFactory::Instance()->registerType("Glider", new GliderCreator());
	TheGameObjectFactory::Instance()->registerType("ShotGlider", new ShotGliderCreator());
	TheGameObjectFactory::Instance()->registerType("RoofTurret", new RoofTurretCreator());
	TheGameObjectFactory::Instance()->registerType("Eskeletor", new EskeletorCreator());
	TheGameObjectFactory::Instance()->registerType("Level1Boss", new Level1BossCreator());

	m_pGameStateMachine = new GameStateMachine();
	m_pGameStateMachine->changeState(new MainMenuState());

	return true;
}

void Game::render() {

	SDL_RenderClear(m_pRenderer); // clear the renderer to the draw color 

	m_pGameStateMachine->render();
	
	SDL_RenderPresent(m_pRenderer); //draw to the screen 
}

void Game::handleEvents() {
	TheInputHandler::Instance()->update();

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN)){
		m_pGameStateMachine->changeState(new PlayState());
	}//end of if statement 

	}


void Game::clean() {
	std::cout << "cleaning game\n";

	TheInputHandler::Instance()->clean();

	m_pGameStateMachine->clean();

	m_pGameStateMachine = 0;
	delete m_pGameStateMachine;

	TheTextureManager::Instance()->clearTextureMap();

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

void Game::update() {

	m_pGameStateMachine->update();

}

Game* Game::Instance() {
	if (s_pInstance == 0)
	{
		s_pInstance = new Game();
		return s_pInstance;
	}
	return s_pInstance;
}

/*void Game::quit() {
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}*/


void Game::setCurrentLevel(int currentLevel)
{
	m_currentLevel = currentLevel;
	m_bLevelComplete = false;
	setPlayerLives(3);
	m_pGameStateMachine->changeState(new PlayState());
	//m_bLevelComplete = false;
}

Game::~Game()
{
	// we must clean up after ourselves to prevent memory leaks
	m_pRenderer = 0;
	m_pWindow = 0;
}



Game::Game() :
	m_pWindow(0),
	m_pRenderer(0),
	m_bRunning(false),
	m_pGameStateMachine(0),
	m_playerLives(3),
	m_scrollSpeed(0.8),
	m_bLevelComplete(false),
	m_bChangingState(false)
{
	// add some level files to an array
	m_levelFiles.push_back("assets/map1.tmx");
	m_levelFiles.push_back("assets/map2.tmx");

	// start at this level
	m_currentLevel = 1;
}