#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
// Pinball Scene Class
class SceneCollision : public SceneBase
{
public:
	// Pinball Scene
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	bool CheckCollision(GameObject* go1, GameObject* go2, float dt);
	void RenderGO(GameObject *go);
	void Bumper(int x,int y,int z, int posx, int posy);
	void ThickWall(int x, int y, int z, int posx, int posy);
	void CollisionResponse(GameObject* go, GameObject* go2);
	void PowerUp(float x, float y);
	GameObject* FetchGO();
	void ReturnGO(GameObject *go);
	void Flipper();
	void Platforms();
protected:
	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	int m_objectCount;
	int ballCount;
	std::vector<GameObject*> activeItems;
	GameObject* powerUp;
	GameObject* m_spring;
	GameObject* Platform;
	GameObject* Platform2;

	GameObject* Pillar;
	GameObject* Pillar2;

	GameObject* Collide;


	//Flipper
	std::vector<GameObject*> flipper;
	float maxAngle = 65;
	float AngleF = 0;
	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	float timer =0;
	bool GameStart = false; // Game start
	bool win = false;
	int score =0;
	int highScore =0;
	bool collectedpowerUp = false;
};
#endif