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
	virtual void Update(float dt);
	virtual void Render();
	virtual void Exit();

	bool CheckCollision(GameObject* go, GameObject* go2);
	void CollisionResponse(GameObject* go, GameObject* go2);
	void RenderGO(GameObject* go);

};

#endif