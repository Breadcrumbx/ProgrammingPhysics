#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}
SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	m_objectCount = 0;
	ballCount = 0;
	m_ghost = new GameObject(GameObject::GO_BALL);

	int n = 30;
	for (int i = 0; i < n+1; i++)
	{
		GameObject* go = FetchGO();
		go->type = GameObject::GO_WALL;
		go->active = true;
		go->scale.Set(3, 6, 1);
		go->normal.Set(cos(Math::DegreeToRadian(90 * i/n)), sin(Math::DegreeToRadian(90 * i / n)), 0);
		go->pos = Vector3(m_worldWidth * 0.79, m_worldHeight * 0.73, 0) + (go->normal * 25);

		GameObject* topLeftCorner = FetchGO();
		topLeftCorner->type = GameObject::GO_WALL;
		topLeftCorner->active = true;
		topLeftCorner->scale.Set(3, 6, 1);
		topLeftCorner->normal.Set(cos(Math::DegreeToRadian(90 + 90 * i / n)), sin(Math::DegreeToRadian(90 + 90 * i / n)), 0);
		topLeftCorner->pos = Vector3(m_worldWidth * (1 - 0.79), m_worldHeight * 0.73, 0) + (topLeftCorner->normal * 25);

		if (i == n)
		{
			for (int j = 1; j < 7; j++)
			{
				GameObject* go2 = FetchGO();
				go2->type = GameObject::GO_WALL;
				go2->active = true;
				go2->scale.Set(3, 6, 1);
				go2->normal.Set(cos(Math::DegreeToRadian(90 * i / n)), sin(Math::DegreeToRadian(90 * i / n)), 0);
				go2->pos = Vector3(m_worldWidth * 0.79 - go2->scale.y * 2 * j, m_worldHeight * 0.73, 0) + (go->normal * 25);
			}
		}
	}
	// Ball 
	/*GameObject* ball = FetchGO();
	ball->type = GameObject::GO_BALL;
	ball->active = true;
	ball->scale.Set(3, 3, 1);
	ball->normal.Set(1, 0, 0);
	ball->pos = Vector3(124.5, 10.0, 0);
	ball->vel = Vector3(Math::RandFloatMinMax(-1, 1) * 0, Math::RandFloatMinMax(-1, 1) * 0, 0);
	activeItems.push_back(ball);
	ballCount += 1;*/
	//Structures
	GameObject* go = FetchGO(); //Wall RIGHT
	go->type = GameObject::GO_WALL;
	go->active = true;
	go->scale.Set(3, 38, 1);
	go->normal.Set(1, 0, 0);
	go->pos = Vector3(m_worldWidth * 0.953, m_worldHeight * 0.3, 0) + (go->normal * (3.375 * 1));

	GameObject* go2 = FetchGO();
	go2->type = GameObject::GO_WALL;
	go2->active = true;
	go2->scale.Set(3, 38, 1);
	go2->normal.Set(1, 0, 0);
	go2->pos = Vector3(m_worldWidth * -0.003, m_worldHeight * 0.3, 0) + (go->normal * (3.375 * 1));

	//One way wall
	GameObject* go3 = FetchGO();
	go3->type = GameObject::GO_ONEWAY;
	go3->active = true;
	go3->scale.Set(1, 3.8, 1);
	go3->normal.Set(0, -1, 0);
	go3->pos = Vector3(m_worldWidth * 0.903, m_worldHeight * 0.60, 0) + (go->normal * (3.375 * 1));
	//Right side wall
	GameObject* go4 = FetchGO();
	go4->type = GameObject::GO_WALL;
	go4->active = true;
	go4->scale.Set(10, 19, 1);
	go4->normal.Set(cos(Math::DegreeToRadian(105)), sin(Math::DegreeToRadian(105)), 0);
	go4->pos = Vector3(m_worldWidth * 0.71, m_worldHeight * 0.06, 0) + (go->normal * (3.375 * 1));
	//Left side wall
	GameObject* go5 = FetchGO();
	go5->type = GameObject::GO_WALL;
	go5->active = true;
	go5->scale.Set(10, 21, 1);
	go5->normal.Set(cos(Math::DegreeToRadian(75)), sin(Math::DegreeToRadian(75)), 0);
	go5->pos = Vector3(m_worldWidth * 0.12, m_worldHeight * 0.07, 0) + (go->normal * (3.375 * 1));

	
	//flipper.push_back(go5); // left wall
	//flipper.push_back(go4); // right wall


	ThickWall(30, 5, 1, m_worldWidth * 0.87, m_worldHeight * 0.3);
	Platforms();
	
	m_spring = new GameObject(GameObject::GO_SPRING);
	m_spring->active = true;
	m_spring->mass = 50;
	m_spring->scale.Set(2, 2, 1);
	m_spring->normal.Set(0, 1, 0);
	m_spring->pos = Vector3(m_worldWidth * 0.905, m_worldHeight * 0.05, 0) + (go->normal * (3.375 * 1));
	m_goList.push_back(m_spring);
	//GameObject* go4 = FetchGO();
	//go4->type = GameObject::GO_PILLAR;
	//go4->active = true;
	//go4->scale.Set(3, 3, 1);
	//go4->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);


	//thick wall
	Bumper(5, 5, 1, m_worldWidth * 0.25, m_worldHeight * 0.5);
	Bumper(5, 5, 1, m_worldWidth * 0.70, m_worldHeight * 0.5);

	Bumper(5, 5, 1, m_worldWidth * 0.50, m_worldHeight * 0.8);
	//for (int i = -1; i <= 1; i += 2)
	//{
	//	GameObject* go2 = FetchGO();
	//	go2->type = GameObject::GO_PILLAR;
	//	go2->scale.Set(go->scale.x, go->scale.x, 1);
	//	go2->pos = Vector3(go->pos.x + i * go->scale.y, go->pos.y, 0);
	//}
}

GameObject* SceneCollision::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (auto go : m_goList)
	{
		if (!go->active)
		{
			go->active = true;
			m_objectCount++;
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GameObject* newGO = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(newGO);
	}
	return FetchGO();
}

void SceneCollision::ReturnGO(GameObject *go)
{
	//Exercise 3: implement ReturnGO()
	go->active = false;
	m_objectCount--;
	return;
}

void SceneCollision::Flipper()
{
	

	//Right flipper
	GameObject* go6 = FetchGO();
	go6->type = GameObject::GO_FLIPPER;
	go6->active = true;
	go6->scale.Set(1, 8, 1);
	go6->normal.Set(cos(Math::DegreeToRadian(105 - AngleF)), sin(Math::DegreeToRadian(105 - AngleF)), 0);
	go6->pos = Vector3(m_worldWidth * 0.57, m_worldHeight * 0.092, 0);/*(go->normal * (3.375 * 1)*/


	//Vector3 RightFlipper = Vector3(go6->normal.y, go6->normal.x);
	////Right Collision pillar
	//GameObject* go7 = FetchGO();
	//go7->type = GameObject::GO_PILLAR;
	//go7->active = true;
	//go7->scale.Set(0.9, 0.9, 1);
	//go7->pos = go6->pos - (RightFlipper * 12);//Vector3(m_worldWidth * 0.487, m_worldHeight * 0.075, 0);//) + (go->normal * (3.375 * 1));
	// Right Pivot
	//GameObject* go8 = FetchGO();
	//go8->type = GameObject::GO_PIVOT;
	//go8->active = true;
	//go8->scale.Set(1.5, 1.5, 5);
	//go8->pos = Vector3(m_worldWidth * 0.59, m_worldHeight * 0.11, 0);// +(go->normal * (3.375 * 1));


	// Left flipper
	GameObject* go9 = FetchGO();
	go9->type = GameObject::GO_FLIPPER;
	go9->active = true;
	go9->scale.Set(1, 8, 1);
	go9->normal.Set(cos(Math::DegreeToRadian(75 + AngleF)), sin(Math::DegreeToRadian(75 + AngleF)), 0);
	go9->pos = Vector3(m_worldWidth * 0.33, m_worldHeight * 0.098, 0);// + (go->normal * (3.375 * 1));


	//Vector3 LeftFlipper = Vector3(go9->normal.y, go9->normal.x);
	//Left Collision pillar
	//GameObject* go10 = FetchGO();
	//go10->type = GameObject::GO_PILLAR;
	//go10->active = true;
	//go10->scale.Set(0.9, 0.9, 1);
	//go10->pos = go9->pos - (LeftFlipper * 12);//Vector3(m_worldWidth * 0.362, m_worldHeight * 0.0755, 0);// + (go->normal * (3.375 * 1));
	//// Left Pivot
	//GameObject* go11 = FetchGO();
	//go11->type = GameObject::GO_PIVOT;
	//go11->active = true;
	//go11->scale.Set(1.5, 1.5, 5);
	//go11->pos = Vector3(m_worldWidth * 0.262, m_worldHeight * 0.11, 0); //+ (go->normal * (3.375 * 1));


	flipper.push_back(go9); // left flipper
	//flipper.push_back(go10); // left pillar
	//flipper.push_back(go11); // left pivot


	flipper.push_back(go6); // right flipper
	//flipper.push_back(go7);//right pillar
	//flipper.push_back(go8); // right pivot
}

void SceneCollision::Platforms()
{
	Platform = new GameObject(GameObject::GO_MOVINGPLATFORM);
	Platform->active = true;
	Platform->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.3, 0);
	Platform->scale = Vector3(1, 12, 1);
	Platform->normal = Vector3(0, 1, 0);
	m_goList.push_back(Platform);

	Platform2 = new GameObject(GameObject::GO_MOVINGPLATFORM);
	Platform2->active = true;
	Platform2->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.3, 0);
	Platform2->scale = Vector3(12, 1, 1);
	Platform2->normal = Vector3(1, 0, 0);
	m_goList.push_back(Platform2);

	Pillar = new GameObject(GameObject::GO_PILLAR);
	Pillar->active = true;
	Pillar->scale = Vector3(1, 1, 1);
	Pillar->pos = Vector3(Platform->pos.x + Platform->scale.y, Platform->pos.y, 0);
	Pillar->normal = Vector3(0, 1, 0);
	m_goList.push_back(Pillar);

	Pillar2 = new GameObject(GameObject::GO_PILLAR);
	Pillar2->active = true;
	Pillar2->scale = Vector3(1, 1, 1);
	Pillar2->pos = Vector3(Platform->pos.x - Platform->scale.y, Platform->pos.y, 0);
	Pillar2->normal = Vector3(1, 0, 0);
	m_goList.push_back(Pillar2);

}

void SceneCollision::Update(double dt)
{
	/*std::cout << std::boolalpha;
	std::cout << win << std::endl;*/
	SceneBase::Update(dt);
	//std::cout << m_spring->pos.y <<std::endl;
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	if (score >= highScore)
	{
		highScore = score;
	}
	
	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0)  && !GameStart && !win)
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / w * m_worldWidth;
		//float posY = 0.5f * m_worldHeight;
		float posY = (h - y) / h * m_worldHeight;
		if ((posX > 47.6667 && posX < 85.3333) && (posY > 50.5 && posY < 77))
		{
			GameStart = true;
			for (int i = 0; i < activeItems.size(); i++)
			{
				activeItems[i]->active = false;
			}
			for (int i = 0; i < m_goList.size(); i++)
			{
				if (m_goList[i]->type == GameObject::GO_POWERUP)
				{
					m_goList[i]->active = false;
				}
			}
			GameObject* ball = FetchGO();
			ball->type = GameObject::GO_BALL;
			ball->active = true;
			ball->scale.Set(3, 3, 1);
			ball->normal.Set(1, 0, 0);
			ball->pos = Vector3(124.5, 10.0, 0);
			ball->vel = Vector3( 0, 0, 0);
			//activeItems.push_back(ball);
			ballCount += 1;
			std::cout << "Ball spawned" << std::endl;
		}
		else if ((posX > 47.6667 && posX < 85.1667) && (posY > 13.1667 && posY < 39.5))
		{
			Application::bQuit = true;
		}
		/*m_ghost->pos = Vector3(posX,posY , 0);
		m_ghost->active = true;*/
		//if (!m_ghost->active)
		//{
		//	m_ghost->active = true;
		//	m_ghost->pos.y = m_worldHeight * 0.5;
		//	m_ghost->pos.x = m_worldWidth * 0.5;
		//	m_ghost->vel.y = 0;
		//}
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / w * m_worldWidth;
		float posY = 0.5f * m_worldHeight;
		//GameObject* go = FetchGO();
		//go->type = GameObject::GO_BALL;
		//go->pos = m_ghost->pos;
		//go->scale = Vector3(2, 2, 1);
		//go->vel = go->pos - Vector3(x / w * m_worldWidth, (h - y) / h * m_worldHeight, 0);
		////go->vel = m_ghost->pos - Vector3(posX, posY, 0);
		//go->mass = 8;
		//m_ghost->vel = go->vel;
		//m_ghost->active = false;


		//Exercise 6: spawn small GO_BALL
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / w * m_worldWidth;
		float posY = (h - y) / h * m_worldHeight;
		std::cout << posX << std::endl;
		std::cout << posY << std::endl;
		m_ghost->pos = Vector3(posX, posY, 0);
		ballCount++;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 10: spawn large GO_BALL
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / w * m_worldWidth;
		float posY = (h - y) / h * m_worldHeight;

		//Exercise 6: spawn big GO_BALL
		GameObject* bigBall = FetchGO();
		bigBall->type = GameObject::GO_BALL;
		bigBall->pos = m_ghost->pos;
		bigBall->vel = m_ghost->pos - Vector3(posX, posY, 0);
		bigBall->scale = Vector3(3, 3, 1);
		bigBall->mass = 27;
	}
	static bool pressed = false;
	if (win)
	{
		if (Application::IsKeyPressed(VK_RETURN) && !pressed)
		{
			pressed = true;
			win = false;
			score = 0;
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && pressed)
		{
			pressed = false;
		}
		//return;
	}
	if (!GameStart)
	{
		return;
	}

	
	

	
	
	
	//Moving platform
	if (Application::IsKeyPressed('A'))
	{
		Platform->vel.x = -100;
		Platform2->vel.x = -100;
		Pillar->vel.x = -100;
		Pillar2->vel.x = -100;
	}
	else if (Application::IsKeyPressed('D'))
	{
		Platform->vel.x = 100;
		Platform2->vel.x = 100;
		Pillar->vel.x = 100;
		Pillar2->vel.x = 100;
	}
	else
	{
		Platform->vel.x = 0;
		Platform2->vel.x = 0;
		Pillar->vel.x = 0;
		Pillar2->vel.x = 0;
	}
	

	// Spring button
	if (Application::IsKeyPressed(VK_SPACE))
	{
		m_spring->pos.y -= 1;
		m_spring->vel.y = 0;
	}
	else
	{
		if (m_spring->pos.y >= m_worldHeight * 0.05)
		{
			m_spring->vel.y = 0;
		}
		else
		{
			m_spring->vel.y = 130;
		}
	}

	if (Application::IsKeyPressed('V'))
	{
		AngleF += 200 * dt;
		if (AngleF > maxAngle)
		{
			AngleF = maxAngle;
		}
	}
	else
	{
		AngleF -= 200 * dt;
		if (AngleF < 0)
		{
			AngleF = 0;
		}
	}
	for (int i = 0; i < flipper.size(); i++)
	{
		ReturnGO(flipper[i]);
	}
	flipper.clear();
	Flipper();
	m_spring->pos.y = Math::Clamp(m_spring->pos.y, m_worldHeight * 0.02f, m_worldHeight * 0.05f);
	//power up
	timer += dt;
	if (timer > 10)
	{
		for (int i = 0; i < 1; i++)
		{
			PowerUp(Math::RandFloatMinMax(14.3333, 126.5), Math::RandFloatMinMax(61.5, 89.3333));
			timer = 0;
		}
	}
	
	if (collectedpowerUp)
	{
		GameObject* newBall = FetchGO();
		newBall->type = GameObject::GO_BALL;
		newBall->active = true;
		newBall->scale.Set(3, 3, 1);
		newBall->normal.Set(1, 0, 0);
		newBall->pos = Vector3(124.5, 10.0, 0);
		newBall->vel = Vector3(Math::RandFloatMinMax(-1, 1) * 0, Math::RandFloatMinMax(-1, 1) * 0, 0);
		activeItems.push_back(newBall);
		collectedpowerUp = false;
		ballCount++;
	}
	//Physics Simulation Section
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Exercise 7: handle out of bound game objects
			go->pos += go->vel * dt * m_speed;
			if ((go->pos.x > m_worldWidth + go->scale.x || go->pos.x < 0 - go->scale.x) ||
				(go->pos.y > m_worldHeight + go->scale.y || go->pos.y < 0 - go->scale.y))
			{
				ReturnGO(go);
				ballCount--;
				if (ballCount <= 0)
				{
					if (score >= 50)
					{
						win = true;
					}
					GameStart = false;
					score = 0;
					timer = 0;
					break;
				}
			}
			if (go->pos.x + go->scale.x > m_worldWidth && go->vel.x > 0 ||
				go->pos.x - go->scale.x < 0 && go->vel.x < 0) {
				go->vel.x *= -1;
			}
			if (go->pos.y + go->scale.y > m_worldHeight && go->vel.y > 0)
			{
				go->vel.y *= -1;
			}
			if (go->type == GameObject::GO_BUMPER && go->scale.x > 5)
			{
				go->scale -= Vector3(0.1, 0.1, 1);
			}
			
			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;
				if (go2->active)
				{
					GameObject* ball = go;
					GameObject* other = go2;
					if (ball->type != GameObject::GO_BALL)
					{
						if (other->type != GameObject::GO_BALL)
							continue;
						ball = go2;
						other = go;
					}
					if (CheckCollision(ball, other, dt))
					{
						CollisionResponse(ball, other);
						continue;
					}
					//Exercise 8b: store values in auditing variables
				}
			}
			//Exercise 8b: store values in auditing variables
			//Exercise 10: handle collision using momentum swap instead
			/*for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;
				Vector3 p1 = go->mass * go->vel;
				Vector3 p2 = go2->mass * go2->vel;
				u1 = go->vel;
				u2 = go2->vel;
				go->vel = p2 * (1.0f / go->mass);
				go2->vel = p1 * (1.0f / go2->mass);
				m1 = go->mass;
				m2 = go2->mass;
				v1 = go->vel;
				v2 = go2->vel;
				break;
			}*/
			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm [solution to be given later] 
		}
		if (go->type == GameObject::GO_BALL)
		{
			go->vel.y -= 98.f * dt;
		}
	}

	Platform->pos.x = Math::Clamp(Platform->pos.x, Platform->scale.y + Pillar->scale.x * 2 + 2, Collide->pos.x - Collide->scale.y * 3.5f);
	Platform2->pos.x = Platform->pos.x;

	Pillar->pos.x = Math::Clamp(Pillar->pos.x, Platform->scale.y * 2 + Pillar->scale.x * 2 + 2, Platform->pos.x + Platform->scale.y);
	Pillar2->pos.x = Math::Clamp(Pillar2->pos.x, Pillar->scale.x * 2 + 2, Platform->pos.x - Platform->scale.y);
}


void SceneCollision::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		//Exercise 11: think of a way to give balls different colors
		break;
	case GameObject::GO_WALL:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = atan2f(go->normal.y, go->normal.x);
		modelStack.Rotate(Math::RadianToDegree(angle), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_FLIPPER:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = atan2f(go->normal.y, go->normal.x);
		modelStack.Rotate(Math::RadianToDegree(angle), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FLIPPER], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_ONEWAY:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = atan2f(go->normal.y, go->normal.x);
		modelStack.Rotate(Math::RadianToDegree(angle), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PIVOT:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PIVOT], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SPRING:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = atan2f(go->normal.y, go->normal.x);
		modelStack.Rotate(Math::RadianToDegree(angle), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPRING], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_BUMPER:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = atan2f(go->normal.y, go->normal.x);
		modelStack.Rotate(Math::RadianToDegree(angle), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUMPER], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_POWERUP:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_MOVINGPLATFORM:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(1.0f, 0.f, 0.f);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	}
	}
}

void SceneCollision::Bumper(int x, int y, int z, int posx, int posy)
{

	//Top n bottom
	GameObject* go = FetchGO();
	go->active = true;
	go->type = GameObject::GO_BUMPER;
	go->scale.Set(x, y, z);
	go->normal.Set(0, 1, 0);
	go->pos = Vector3(posx,  posy, 0);
		//Left and Right
	GameObject* go2 = FetchGO();
	go2->active = true;
	go2->type = GameObject::GO_BUMPER;
	go2->scale.Set(x, y , z);
	go2->normal.Set(1, 0, 0);
	go2->pos = Vector3( posx,  posy, 0);

	/*for (int i = -1; i <= 1; i += 2)
	{
		GameObject* go3 = FetchGO();
		go3->type = GameObject::GO_PILLAR;
		go3->active = true;
		go3->scale.Set(1, 1, 1);
		go3->pos = Vector3(go->pos.x + i * go->scale.y * 0.80f, go->pos.y - i * go->scale.y * 0.80f, 0);
	}
	for (int i = -1; i <= 1; i += 2)
	{
		GameObject* go4 = FetchGO();
		go4->type = GameObject::GO_PILLAR;
		go4->active = true;
		go4->scale.Set(1, 1, 1);
		go4->pos = Vector3(go->pos.x + i * go->scale.y * 0.80f, go->pos.y + i * go->scale.y * 0.80f, 0);
	}*/
	return;
}

void SceneCollision::ThickWall(int x, int y, int z, int posx, int posy)
{

	//Top n bottom
	GameObject* go = FetchGO();
	go->active = true;
	go->type = GameObject::GO_WALL;
	go->scale.Set(x, y, z);
	go->normal.Set(0, 1, 0);
	go->pos = Vector3(posx, posy, 0);

	Collide = go;
	//Left and Right
	GameObject* go2 = FetchGO();
	go2->active = true;
	go2->type = GameObject::GO_WALL;
	go2->scale.Set(y, x, z);
	go2->normal.Set(1, 0, 0);
	go2->pos = Vector3(posx, posy, 0);

	for (int i = -1; i <= 1; i += 2)
	{
		GameObject* go3 = FetchGO();
		go3->type = GameObject::GO_PILLAR;
		go3->active = true;
		go3->scale.Set(1, 1, 1);
		go3->pos = Vector3(go->pos.x + i * go->scale.y * 0.80f, go->pos.y - i * go->scale.y * 5.8f, 0);
	}
	for (int i = -1; i <= 1; i += 2)
	{
		GameObject* go4 = FetchGO();
		go4->type = GameObject::GO_PILLAR;
		go4->active = true;
		go4->scale.Set(1, 1, 1);
		go4->pos = Vector3(go2->pos.x + i * go->scale.y * 0.80f, go2->pos.y + i * go->scale.y * 5.8f, 0);
	}
	return;
}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);
	
	if (!GameStart)
	{
		RenderMeshOnScreen(meshList[GEO_MENU], 40, 30, 5, 5);
	}
	else
	{
		if (m_ghost->active)
		{
			RenderGO(m_ghost);
		}

		if (m_spring->active)
		{
			RenderGO(m_spring);
		}

		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}

		//On screen text

		//Exercise 5: Render m_objectCount

		//Exercise 8c: Render initial and final momentum
		std::ostringstream ss;
		ss << "Score : " << score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.467, 0.533, 0.600), 3, 0, 9);

		ss.str("");
		ss << "High Score: " << highScore;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.467, 0.533, 0.600), 3, 0, 6);

		std::ostringstream ss2;
		ss2.precision(3);
		ss2 << "Object Count: " << m_objectCount;
		RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0.467, 0.533, 0.600), 3, 0, 0);

		ss2.str("");
		ss2 << "Ball Count: " << ballCount;
		RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0.467, 0.533, 0.600), 3, 0, 3);
		ss.str("");
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 57);

		//RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
		
	}
	
	if (win)
	{
		RenderMeshOnScreen(meshList[GEO_WIN], 40, 30, 5, 5);
	}
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}

bool SceneCollision::CheckCollision(GameObject* go, GameObject* go2, float dt)
{
	if (go2->type == GameObject::GO_BALL)
	{
		Vector3 displacement = go2->pos - go->pos;
		float combinedRadii = go->scale.x + go2->scale.x;
		if ((displacement.LengthSquared() < combinedRadii * combinedRadii) &&
			(go2->vel - go->vel).Dot(go2->pos - go->pos) < 0) //prevents internal collision with ball and ball
			return true;
	}
	else if (go2->type == GameObject::GO_WALL || go2->type == GameObject::GO_SPRING || go2->type == GameObject::GO_FLIPPER || go2->type == GameObject::GO_MOVINGPLATFORM)
	{
		Vector3 N = go2->normal;
		Vector3 NP = Vector3(N.y, -N.x, 0);
		Vector3 w0_b1 = go2->pos - go->pos;
		float r = go->scale.x;
		float h_2 = go2->scale.x;
		float l_2 = go2->scale.y;

		if (w0_b1.Dot(N) < 0) //flip normal based on ball dir
			N = -N;

		//if ((w0_b1).Dot(N) < r + h_2 * 0.5 &&
		//	Math::FAbs((w0_b1).Dot(NP)) < r + l_2 * 0.5 &&
		//	N.Dot(go->vel) > 0) //prevents internal collision with ball and wall
		//{
		//	return true;
		//}
		if (w0_b1.Dot(N) < r + h_2 && abs(w0_b1.Dot(NP)) < l_2 && go->vel.Dot(N) > 0)
			return true;
	}
	else if (go2->type == GameObject::GO_PILLAR || go2->type == GameObject::GO_BUMPER || go2->type == GameObject::GO_POWERUP || go2->type == GameObject::GO_PIVOT)
	{
		Vector3 u = go->vel;
		Vector3 p2_p1 = go2->pos - go->pos;
		float r1 = go->scale.x;
		float r2 = go2->scale.x;

		if (p2_p1.Length() < r1 + r2 &&
			p2_p1.Dot(u) > 0) //prevents internal collision with ball and pillar
		{
			std::cout << "COLLIDED" << std::endl;
			return true;
		}
	}
	else if (go2->type == GameObject::GO_ONEWAY)
	{
		Vector3 N = go2->normal;
		Vector3 NP = Vector3(N.y, -N.x, 0);
		Vector3 w0_b1 = go2->pos - go->pos;
		float r = go->scale.x;
		float h_2 = go2->scale.x;
		float l_2 = go2->scale.y;

		if (Math::FAbs(w0_b1.Dot(N)) < r + h_2 &&
			Math::FAbs(w0_b1.Dot(NP)) < l_2 &&
			go->vel.Dot(N) > 0)
		{
			std::cout << "COLLIDED" << std::endl;
			return true;
		}
	}
	return false;
}

void SceneCollision::CollisionResponse(GameObject* go, GameObject* go2)
{
	if (go2->type == GameObject::GO_BALL)
	{
		m1 = go->mass;
		m2 = go2->mass;
		u1 = go->vel;
		u2 = go2->vel;

		float sqDist = (go->pos - go2->pos).LengthSquared();
		float dp = (u1 - u2).Dot(go->pos - go2->pos) / sqDist;
		v1 = u1 - 2 * m2 / (m1 + m2) * dp * (go->pos - go2->pos);
		v2 = u2 - 2 * m1 / (m1 + m2) * dp * (go2->pos - go->pos);

		go->vel = v1;
		go2->vel = v2;
	}
	else if (go2->type == GameObject::GO_POWERUP)
	{
		go2->active = false;
		collectedpowerUp = true;
		score += 1;
	}
	else if (go2->type == GameObject::GO_WALL || go2->type == GameObject::GO_ONEWAY )
	{
		Vector3 N = go2->normal;
		Vector3 u = go->vel;
		go->vel = u - (2 * u.Dot(N)) * N * 0.9;
	}
	else if (go2->type == GameObject::GO_BUMPER)
	{
		Vector3 N = (go2->pos - go->pos).Normalized();
		Vector3 u = go->vel;
		go->vel = u - (2 * u.Dot(N)) * N * 1.1;
		go2->scale = Vector3(5.5, 5.5, 1);
		score += 2;
	}
	else if (go2->type == GameObject::GO_FLIPPER)
	{
		Vector3 N = go2->normal;
		Vector3 u = go->vel;
		go->vel = u - (2 * u.Dot(N)) * N * 1.1;
		if (go->vel.Length() > 120)
		{
			go->vel = go->vel.Normalized() * 120;
		}
	}
	else if (go2->type == GameObject::GO_SPRING)
	{
		Vector3 N = go2->normal;
		Vector3 u = go->vel;
		if (go2->vel.y > 0)
		{
			m1 = go->mass;
			m2 = go2->mass;
			u1 = go->vel;
			u2 = go2->vel;

			Vector3 N = (go->pos - go2->pos).Normalized();
			float mass = (2 * m2 / (m1 + m2));

			v1 = u1 + mass * ((u2 - u1).Dot(N) * N);
			go->vel = v1;
		}
		else
		{
			go->vel = u - (2 * u.Dot(N)) * N * 0.6;
			Vector3 w0_b1 = go2->pos - go->pos;
			if (w0_b1.Dot(N) > 0)
				N = -N;

			Vector3 projection = (w0_b1.Dot(N) / N.Dot(N)) * N; //find the projection of the direction vector on the normal
			if (projection.Length() < go2->scale.x + go->scale.x)
			{
				Vector3 line = w0_b1 - projection;
				go->pos = go2->pos - line + N * (go2->scale.x + go->scale.x); //set the ball to the top of the wall in the direction of the wall's normal
			}
		}
	}
	else if (go2->type == GameObject::GO_PILLAR || go2->type == GameObject::GO_PIVOT)
	{
		Vector3 N = (go2->pos - go->pos).Normalized();
		Vector3 u = go->vel;
		go->vel = u - (2 * u.Dot(N)) * N * 0.9;
	}
	else if (go2->type == GameObject::GO_MOVINGPLATFORM)
	{

		Vector3 N = go2->normal;
		Vector3 u = go->vel;
		go->vel = u - (2 * u.Dot(N)) * N * 3.5;
		if (go->vel.Length() >= 140)
		{
			go->vel = go->vel.Normalized() * 140;
		}
	}
}

void SceneCollision::PowerUp(float x, float y)
{
	powerUp = new GameObject(GameObject::GO_POWERUP);
	powerUp->active = true;
	powerUp->pos = Vector3(x, y, 1);
	powerUp->scale = Vector3(5, 5, 2);
	powerUp->normal = Vector3(1, 0, 0);
	m_goList.push_back(powerUp);
}