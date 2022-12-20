#ifndef SCENE_ASTEROID_H
#define SCENE_ASTEROID_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAsteroid : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;
	static const int MAX_ANGULAR_SPEED = 2;
public:
	SceneAsteroid();
	~SceneAsteroid();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ship;
	Vector3 m_force;
	Vector3 m_accel;
	int m_objectCount;
	int m_lives;
	int health;
	int m_score;
	int m_bullet;
	bool bossSpawn = false;
	bool vulnerable = true; // vulnerability
	bool shotgunMode = false; // shotgun
	bool attackMode = false; // Attack up power up
	bool GameStart = false; // Game start
	bool rapidfire = false; // machine gun power up
	bool win = false; // Win screen
	bool lose = false;// Lose screen
	float powerTime = 0; // power timer
	float globalTimer = 0; // global timer
	float timer = 0;// timer
	float bigAsteroid = 0; // asteroid timer
	float Etimer = 0;// enemy timer for when it spawns
	float iframes =0; // invincibility frames
	float SMode = 0; // shotgun mode timer
	float AttackTimer = 0; // Attack timer
	float rapidtimer = 0;
	float bosstime = 0;
	Vector3 m_torque;
};

#endif