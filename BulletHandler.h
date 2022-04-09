#pragma once
#include <vector>
#include "Bullet.h"

class BulletHandler {

public:
	static BulletHandler* Instance();

	void addPlayerBullet(int x, int y, int width, int height,
		std::string textureID, int numFrames, Vector2D heading);

	void addEnemyBullet(int x, int y, int width, int height,
		std::string textureID, int numFrames, Vector2D heading);

	const std::vector<PlayerBullet*> getPlayerBullets() { return m_playerBullets; }

	const std::vector<EnemyBullet*> getEnemyBullets() { return m_enemyBullets; }

	void updateBullets();

	void drawBullets();

	void clearBullets();

private:

	static BulletHandler* s_pInstance;

	std::vector<PlayerBullet*> m_playerBullets;
	std::vector<EnemyBullet*> m_enemyBullets;

};


typedef BulletHandler TheBulletHandler;