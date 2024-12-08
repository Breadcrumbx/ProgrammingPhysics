#include "SceneSimulation.h"
#include <GL/glew.h>
#include "Application.h"
#include <sstream>
SceneSimulation::SceneSimulation()
{
	// Empty constructor
}

SceneSimulation::~SceneSimulation()
{
	// Empty destructor
}

void SceneSimulation::Init()
{
	SceneBase::Init();
}

void SceneSimulation::Update(float dt)
{
}

void SceneSimulation::Render()
{
}

void SceneSimulation::Exit()
{
}

bool SceneSimulation::CheckCollision(GameObject* go, GameObject* go2)
{
	return false;
}

void SceneSimulation::CollisionResponse(GameObject* go, GameObject* go2)
{
}

void SceneSimulation::RenderGO(GameObject* go)
{
}
