#ifndef SCENE_SIMULATION_H
#define SCENE_SIMULATION_H

#include "SpatialPartition/Grid.h"
#include "GameObject.h"
#include "SceneBase.h"
#include <vector>
#include <memory>
class SceneSimulation : public SceneBase
{
public:
	SceneSimulation();
	~SceneSimulation();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void UpdateCollision(std::unique_ptr<Grid>& grid);
	bool CheckCollision(GameObject* go, GameObject* go2);
	void CheckCollision(GameObject* go, std::vector<GameObject*>& gameObjectsInCell, int startingIndex);
	void CollisionResponse(GameObject* go, GameObject* go2);
	void RenderGO(GameObject* go);
private:
	std::vector<GameObject*> m_goList;
	// Grid for spatial partition
	std::unique_ptr<Grid> m_grid;
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