#ifndef SCENE_SIMULATION_H
#define SCENE_SIMULATION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneSimulation : public SceneBase
{
public:
	SceneSimulation();
	~SceneSimulation();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool CheckCollision(GameObject* go, GameObject* go2);
	void CollisionResponse(GameObject* go, GameObject* go2);
	void RenderGO(GameObject* go);
private:
	std::vector<GameObject*> m_goList;
	// Speed of simulation
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	// Number of gameobjects in scene
	unsigned int m_count;
	Vector3 m_gravity;
	// Ghost object
	GameObject* m_ghost;
};

#endif