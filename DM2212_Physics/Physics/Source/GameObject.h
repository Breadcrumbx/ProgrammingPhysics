#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct Cell;

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_WALL,
		GO_SPRING, // Spring
		GO_BUMPER, // BUMPER
		GO_PILLAR,
		GO_ONEWAY,
		GO_FLIPPER,
		GO_PIVOT,
		GO_MOVINGPLATFORM,
		GO_ASTEROID, // asteroid
		GO_BIGASTEROID, // big asteroid
		GO_SHIP,// player ship
		GO_BULLET,// player bullet
		GO_ENEMY,// Enemy
		GO_BOSS, // Boss
		GO_EBULLET,// Enemy bullet
		GO_BBULLET, // Boss bullet
		GO_POWERUP,
		GO_POWERUP1, // power up
		GO_POWERUP2,
		GO_POWERUP3,
		GO_POWERUP4,
		GO_POWERUP5,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	Cell* currentCell = nullptr;
	unsigned int cellVectorIndex = -1;
	bool active;
	float mass;
	int hp;
	int rng;
	Vector3 dir;
	float momentOfInertia;
	float angularVelocity;
	float Stimer; // enemy shoot timer
	float Btimer;
	bool split;
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif