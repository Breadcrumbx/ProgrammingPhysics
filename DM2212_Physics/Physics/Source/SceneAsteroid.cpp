#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();
	/* initialize random seed: */
	srand(time(NULL));
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (int i = 0; i < 100; i++)
	{
		GameObject* asteroid = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(asteroid);
	}
	//Exercise 2b: Initialize m_lives and m_score
	m_lives = 3;
	m_score = 0;
	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->mass = 1.f;
	m_ship->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
	m_ship->momentOfInertia = m_ship->mass * m_ship->scale.x * m_ship->scale.x;
	m_ship->angularVelocity = 0.f;
	m_ship->hp = 5; // Ship's HP
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (auto go : m_goList)
	{
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GameObject* newGO = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(newGO);
	}
	return FetchGO();
}

void SceneAsteroid::Update(double dt)
{
	SceneBase::Update(dt);
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = x / w * m_worldWidth;
	float posY = (h - y) / h * m_worldHeight;



	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0) && !GameStart && !win && !lose)
	{
		bLButtonState = true;
		if ((posX > 45.8333 && posX < 87.3333) && (posY > 37.1667 && posY < 63.8333))
		{
			GameStart = true;
		}
		else if ((posX > 45.8333 && posX < 87.3333) && (posY > 6.83333 && posY < 33.6667))
		{
			Application::bQuit = true;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}

	static bool pressed = false;
	if (win)
	{
		if (Application::IsKeyPressed(VK_RETURN) && !pressed)
		{
			pressed = true;
			m_score = 0;
			globalTimer = 0;
			win = false;
			GameStart = false;
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && pressed)
		{
			pressed = false;
		}
		else
		{
			return;
		}
	}
	if (lose)
	{
		if (Application::IsKeyPressed(VK_RETURN) && !pressed)
		{
			pressed = true;
			globalTimer = 0;
			lose = false;
			GameStart = false;
		}
		else if (!Application::IsKeyPressed(VK_RETURN) && pressed)
		{
			pressed = false;
		}
		else
		{
			return;
		}
	}
	if (!GameStart)
	{
		return;
	}
	globalTimer += dt;
	timer += dt;
	Etimer += dt;
	powerTime += dt;
	bosstime += dt;
	bigAsteroid += dt;

	// Invincibility
	if (iframes > 0)
	{
		iframes -= dt;
	}
	if (iframes <= 0)
	{
		iframes = 0;
		vulnerable = true;
	}
	//Shotgun mode
	if (SMode > 0)
	{
		SMode -= dt;
	}
	if (SMode <= 0)
	{
		SMode = 0;
		shotgunMode = false;
	}
	// Rapid fire mode
	if (rapidtimer > 0)
	{
		rapidtimer -= dt;
	}
	if (rapidtimer <= 0)
	{
		rapidtimer = 0;
		rapidfire = false;
	}
	// Attack power
	if (AttackTimer > 0)
	{
		AttackTimer -= dt;
	}
	if (AttackTimer <= 0)
	{
		AttackTimer = 0;
		attackMode = false;
	}
	
	if (timer > 6)
	{
		for (int num_asteroid = 0; num_asteroid < 2; ++num_asteroid) // small asteroid
		{
			GameObject* go = FetchGO();
			go->active = true;
			go->type = GameObject::GO_ASTEROID;
			go->scale = Vector3(3.5, 3.5, 1);
			go->hp = 1;
			go->pos.Set(Math::RandFloatMinMax(-10.0f, m_worldWidth - 100), Math::RandFloatMinMax(-10.0f, m_worldHeight - 100), 0);
			go->vel.Set(rand() % 10 - 5, rand() % 10 - 5, 0); //random range between -5 - 5
			timer = 0;
		}
	}
	if (bigAsteroid > 10)
	{
		for (int num_asteroid = 0; num_asteroid < 1; ++num_asteroid) // big asteroid
		{
			GameObject* go = FetchGO();
			go->active = true;
			go->type = GameObject::GO_BIGASTEROID;
			go->scale = Vector3(6, 6, 1);
			go->hp = 4;
			go->pos.Set(Math::RandFloatMinMax(-10.0f, m_worldWidth - 100), Math::RandFloatMinMax(-10.0f, m_worldHeight - 100), 0);
			go->vel.Set(rand() % 10 - 5, rand() % 10 - 5, 0); //random range between -5 - 5
			bigAsteroid = 0;
		}
	}

	if (Etimer > 15.0)
	{
		for (int enemy = 0; enemy < 1; ++enemy) // enemy spawn codee
		{
			GameObject* temp;
			temp = FetchGO();
			temp->active = true;
			temp->type = GameObject::GO_ENEMY;
			temp->pos.Set(Math::RandFloatMinMax(0.0f, m_worldWidth - 100), Math::RandFloatMinMax(0.0f, m_worldHeight - 100), 0);
			//temp->vel.Set(rand() % 10 - 5, rand() % 10 - 5, 0);// speed of enemy chasing u
			temp->scale = Vector3(6, 6, 1);
			temp->mass = 15;
			temp->hp = 3;
			Etimer = 0;
		}	
	}
	
	if (powerTime > 12)
	{
		for (int powerup = 0; powerup < 1; ++powerup)
		{
			GameObject* temp;
			temp = FetchGO();
			temp->active = true;
			temp->rng = rand() % 4; // 0-4
			switch (temp->rng)
			{
			case 0:
				temp->type = GameObject::GO_POWERUP1;
				break;
			case 1:
				temp->type = GameObject::GO_POWERUP2;
				break;
			case 2:
				temp->type = GameObject::GO_POWERUP3;
				break;
			case 3:
				temp->type = GameObject::GO_POWERUP4;
				break;
			case 4:
				temp->type = GameObject::GO_POWERUP5;
				break;
			}
			temp->pos.Set(Math::RandFloatMinMax(-10.0f, m_worldWidth - 100), Math::RandFloatMinMax(-10.0f, m_worldHeight - 100), 0);
			temp->vel.Set(rand() % 10 - 5, rand() % 10 - 5, 0); //random range between -5 - 5
			temp->scale = Vector3(6, 6, 1);
			temp->mass = 15;
			powerTime = 0;
		}
	}
	
	if (bosstime > 30 && !bossSpawn)
	{
		for (int enemy = 0; enemy < 1; ++enemy) // enemy spawn codee
		{
			GameObject* temp;
			temp = FetchGO();
			temp->active = true;
			temp->type = GameObject::GO_BOSS;
			temp->pos.Set(Math::RandFloatMinMax(0.0f, m_worldWidth - 100), Math::RandFloatMinMax(0.0f, m_worldHeight - 100), 0);
			temp->scale = Vector3(12, 12, 1);
			temp->mass = 15;
			temp->hp = 10;
			bosstime = dt;
			bossSpawn = true;
		}
	}
	m_force.SetZero();
	m_torque.SetZero();
	//Exercise 6: set m_force values based on WASD
	if (Application::IsKeyPressed('W'))
	{
		m_force += 100 * m_ship->dir;
		m_torque = Vector3(0, -1, 0).Cross(Vector3(0, 100, 0)); 
	}
	if (Application::IsKeyPressed('A'))
	{
		m_force += 5 * m_ship->dir;
		m_torque = Vector3(1, -1, 0).Cross(Vector3(0, 5, 0)); 
	}
	if (Application::IsKeyPressed('S'))
	{
		m_force += -100 * m_ship->dir; 
		m_torque = Vector3(0, 1, 0).Cross(Vector3(0, -100, 0));  
	}
	if (Application::IsKeyPressed('D'))
	{
		m_force += 5 * m_ship->dir;
		m_torque = Vector3(-1, -1, 0).Cross(Vector3(0, 5, 0)); 
	}
	//Exercise 11: use a key to spawn some asteroids


	// -----------------------------------BULLET CODE
	//Exercise 14: use a key to spawn a bullet
	static double spaceButtonDelay = 0;
	if (spaceButtonDelay <= 0 && Application::IsKeyPressed(VK_SPACE))
	{
		//m_bullet--;
		if (!rapidfire)
		{
			spaceButtonDelay = .2;
		}
		else
		{
			spaceButtonDelay = .1;
		}
		GameObject* go = FetchGO();
		if (go)
		{
			go->active = true;
			go->type = GameObject::GO_BULLET;
			go->scale.Set(.2f, .2f, .2f);
			go->dir = m_ship->dir;
		}
		try
		{
			go->pos = m_ship->pos + m_ship->dir * m_ship->scale.Length();
			go->vel = m_ship->dir * BULLET_SPEED;
		}
		catch (DivideByZero ex)
		{
			go->pos = m_ship->pos + Vector3(1, 0) * m_ship->scale.Length();
			go->vel = Vector3(1, 0) * BULLET_SPEED;
		}
		if (shotgunMode)
		{
			GameObject* bullet = FetchGO();
			if (bullet)
			{
				bullet->active = true;
				bullet->type = GameObject::GO_BULLET;
				bullet->scale.Set(.2f, .2f, .2f);
				bullet->dir = m_ship->dir;
			}
			Vector3 Right = m_ship->dir.Cross(Vector3(0, 0, 1));
			bullet->pos = m_ship->pos + Right * 1.5 + m_ship->dir * m_ship->scale.Length();
			bullet->vel = m_ship->dir * BULLET_SPEED;
		}

	}
	else if (spaceButtonDelay > 0) spaceButtonDelay -= dt * m_speed;
	//Exercise 15: limit the spawn rate of bullets
	

	//Physics Simulation Section

	//Exercise 7: Update ship's velocity based on m_force
	m_accel = m_force * (1.f / m_ship->mass);

	m_ship->vel += m_accel * dt * m_speed;
	m_ship->pos += m_ship->vel * dt * m_speed;



	if (m_ship->vel.Length() > MAX_SPEED)
		m_ship->vel = m_ship->vel.Normalized() * MAX_SPEED;
	m_ship->pos += m_ship->vel * dt * m_speed;
	//Prac 3, ex 4
	m_ship->angularVelocity += m_torque.z / m_ship->momentOfInertia * dt * m_speed;
	m_ship->angularVelocity = Math::Clamp(m_ship->angularVelocity, -(float)MAX_ANGULAR_SPEED, (float)MAX_ANGULAR_SPEED);
	float dTheta = m_ship->angularVelocity * dt * m_speed;
	m_ship->dir.Set(m_ship->dir.x* cosf(dTheta) - m_ship->dir.y * sinf(dTheta), m_ship->dir.x* sinf(dTheta) + m_ship->dir.y * cosf(dTheta));

	
	if (m_ship->vel.x > MAX_SPEED)
		m_ship->vel.x = MAX_SPEED;
	else if (m_ship->vel.x < -MAX_SPEED)
		m_ship->vel.x = -MAX_SPEED;
	if (m_ship->vel.y > MAX_SPEED)
		m_ship->vel.y = MAX_SPEED;
	else if (m_ship->vel.y < -MAX_SPEED)
		m_ship->vel.y = -MAX_SPEED;

	//Exercise 9: wrap ship position if it leaves screen
	if (m_ship->pos.x > m_worldWidth + m_ship->scale.x * 0.5f)
	{
		m_ship->pos.x = 0 - m_ship->scale.x * 0.5f;
	}
	else if (m_ship->pos.x < 0 - m_ship->scale.x * 0.5f)
	{
		m_ship->pos.x = m_worldWidth + m_ship->scale.x * 0.5f;
	}
	if (m_ship->pos.y > m_worldHeight + m_ship->scale.y * 0.5f)
	{
		m_ship->pos.y = 0 - m_ship->scale.y * 0.5f;
	}
	else if (m_ship->pos.y < 0 - m_ship->scale.y * 0.5f)
	{
		m_ship->pos.y = m_worldHeight + m_ship->scale.y * 0.5f;
	}



	// Collision code
	for(std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if(go->active)
		{
			go->pos += go->vel * dt * m_speed;
			//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
			if (go->type == GameObject::GO_ASTEROID && vulnerable)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				if (displacement.LengthSquared() < combinedRadii * combinedRadii)
				{
					go->active = false;
					//m_ship->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0); // reset the position
					m_ship->vel = Vector3(0,0,0);
					m_ship->hp--; // Minus Ship's health
					break;
				}
			}
			if (go->type == GameObject::GO_BIGASTEROID && vulnerable)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				if (displacement.LengthSquared() < combinedRadii * combinedRadii)
				{
					go->active = false;
					m_ship->vel = Vector3(0, 0, 0);
					m_ship->hp -= 2;
					break;
				}
			}
			if (go->type == GameObject::GO_ENEMY)
			{
				go->vel = Vector3(m_ship->pos.x - go->pos.x, m_ship->pos.y - go->pos.y, 0).Normalized() * 6;// speed of enemy chasing u
			}
			if (go->type == GameObject::GO_ENEMY)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				go->dir = displacement;
				go->Stimer += dt;
				if (displacement.LengthSquared() < combinedRadii * combinedRadii && vulnerable)
				{
					go->active = false;
					m_ship->vel = Vector3(0, 0, 0);
					m_ship->hp--;
					break;
				}
				if (go->Stimer > 2)
				{
					GameObject* bullet = FetchGO();
					if (bullet)
					{
						bullet->active = true;
						bullet->type = GameObject::GO_EBULLET;
						bullet->scale.Set(0.2f, 0.2f, 1.f);
						bullet->vel = go->dir;
						bullet->pos = go->pos;
						go->Stimer = 0;
					}
				}
			}
			if (go->type == GameObject::GO_BOSS)
			{
				go->vel = Vector3(m_ship->pos.x - go->pos.x, m_ship->pos.y - go->pos.y, 0).Normalized() * 6;// speed of enemy chasing u
			}
			if (go->type == GameObject::GO_BOSS)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				go->dir = displacement;
				go->Btimer += dt;
				if (displacement.LengthSquared() < combinedRadii * combinedRadii && vulnerable)
				{
					go->hp--;
					m_ship->vel = Vector3(0, 0, 0);
					m_ship->hp-=5;
					break;
				}
				if (go->Btimer > 2)
				{
					GameObject* bullet = FetchGO();
					if (bullet)
					{
						bullet->active = true;
						bullet->type = GameObject::GO_BBULLET;
						bullet->scale.Set(2.f, 2.f, 1.f);
						bullet->vel = go->dir;
						bullet->pos = go->pos;
						go->Btimer = 0;
					}
				}
			}
			// -----------------------------------------ENEMY BULLET COLLISION
			if (go->type == GameObject::GO_EBULLET && vulnerable)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				if (displacement.LengthSquared() < combinedRadii * combinedRadii)
				{
					go->active = false;
					m_ship->hp--;
					break;
				}
			}
			if (go->type == GameObject::GO_BBULLET && vulnerable)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				if (displacement.LengthSquared() < combinedRadii * combinedRadii)
				{
					go->active = false;
					m_ship->hp -= 2;
					break;
				}
			}
			// --------------------------------------------POWERUP CODE
			if (go->type == GameObject::GO_POWERUP1 || go->type == GameObject::GO_POWERUP2 || go->type == GameObject::GO_POWERUP3 || go->type == GameObject::GO_POWERUP4 || go->type== GameObject::GO_POWERUP5)
			{
				float combinedRadii = go->scale.x + m_ship->scale.x;
				Vector3 displacement = m_ship->pos - go->pos;
				go->dir = displacement;
				
				if (displacement.LengthSquared() < combinedRadii * combinedRadii && go->type == GameObject::GO_POWERUP1)
				{
					iframes = 8.0;
					go->active = false;
					vulnerable = false;
					break;
				}
				else if (displacement.LengthSquared() < combinedRadii * combinedRadii && go->type == GameObject::GO_POWERUP2)
				{
					SMode = 10.0;
					shotgunMode = true;
					go->active = false;
					break;
				}
				else if (displacement.LengthSquared() < combinedRadii * combinedRadii && go->type == GameObject::GO_POWERUP3)
				{
					AttackTimer = 8.0;
					attackMode = true;
					go->active = false;
					break;
				}
				else if (displacement.LengthSquared() < combinedRadii * combinedRadii && go->type == GameObject::GO_POWERUP4)
				{
					m_ship->hp++;
					go->active = false;
					break;
				}
				else if (displacement.LengthSquared() < combinedRadii * combinedRadii && go->type == GameObject::GO_POWERUP5)
				{
					rapidfire = true;
					rapidtimer = 5.0;
					go->active = false;
					break;
				}

			}
			//Exercise 13: asteroids should wrap around the screen like the ship
			if (go->type == GameObject::GO_ASTEROID || go->type == GameObject::GO_POWERUP1 || go->type == GameObject::GO_POWERUP2 || go->type == GameObject::GO_POWERUP3 || go->type == GameObject::GO_POWERUP4 || go->type == GameObject::GO_POWERUP5 || go->type == GameObject::GO_BIGASTEROID || go->type == GameObject::GO_BOSS || go->type == GameObject::GO_ENEMY)
			{
				go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x * 0.5f, m_worldWidth + go->scale.x * 0.5f);
				go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y * 0.5f, m_worldHeight + go->scale.y * 0.5f);
			}
			
			//Exercise 16: unspawn bullets when they leave screen
			if (go->pos.x < 0 - go->scale.x * .5f || go->pos.x > m_worldWidth + go->scale.x * .5f ||
				go->pos.y < 0 - go->scale.y * .5f || go->pos.y > m_worldHeight + go->scale.y * .5f)
			{
				go->active = false;
			}
			//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
			if (go->type == GameObject::GAMEOBJECT_TYPE::GO_BULLET)
			{
				for (unsigned i = 0; i < m_goList.size(); i++)
				{
					if (go != m_goList[i] && m_goList[i]->active && m_goList[i]->type != GameObject::GAMEOBJECT_TYPE::GO_BULLET && m_goList[i]->hp > 0 && m_goList[i]->type != GameObject::GAMEOBJECT_TYPE::GO_POWERUP1 && m_goList[i]->type != GameObject::GAMEOBJECT_TYPE::GO_POWERUP2 && m_goList[i]->type != GameObject::GAMEOBJECT_TYPE::GO_POWERUP3 && m_goList[i]->type != GameObject::GAMEOBJECT_TYPE::GO_POWERUP4 && m_goList[i]->type != GameObject::GAMEOBJECT_TYPE::GO_POWERUP5)
					{
						float dist = (go->pos - m_goList[i]->pos).Length();

						if (dist < go->scale.x / 2 + m_goList[i]->scale.x / 2)
						{
							go->active = false; //Remove bullet
							//m_bullet--;
							m_score++;
							//Changing of asteroid variables
							if (attackMode)
							{
								m_goList[i]->hp-=2;//Minus 2hp
							}
							else
							{
								m_goList[i]->hp--;//Minus hp
							}
							if (m_goList[i]->hp <= 0)
							{
								m_goList[i]->active = false;
							}
							if (m_goList[i]->type == GameObject::GAMEOBJECT_TYPE::GO_BIGASTEROID && m_goList[i]->hp <= 0) // splitting code
							{
								Vector3 TotalMomentum;
								Vector3 AccMomentum;
								TotalMomentum = m_goList[i]->mass * m_goList[i]->vel;
								for (int j = 0; j < 3; j++)
								{
									GameObject* go1 = FetchGO();
									go1->active = true;
									go1->type = GameObject::GO_ASTEROID;
									go1->scale = Vector3(3, 3, 1);
									go1->hp = 1;
									go1->mass = m_goList[i]->mass / 3;
									if (j < 2)
									{
										go1->vel.Set(rand() % 20 - 5, rand() % 20 - 5, 0);
										AccMomentum += go1->mass * go1->vel;
									}
									else
									{
										go1->vel = (TotalMomentum - AccMomentum) * (1 / go1->mass);
									}
									float PosScale;
									PosScale = (0.5 * m_goList[i]->scale.x) - (0.5 * go1->scale.x);
									if (go1->vel.Length() > 0)
									{
										go1->pos = m_goList[i]->pos + (go1->vel.Normalized() * PosScale);
									}
									else
									{
										go1->pos = m_goList[i]->pos;
									}
								}
							}
							break;
						}
					}
				}
			}
		}
	}
	if (m_lives <= 0)
	{
		m_score = 0; // reset
		m_lives = 3;
		powerTime = 0;
		timer = 0;
		Etimer = 0;
		iframes = 0;
		SMode = 0;
		AttackTimer = 0;
		globalTimer = 0;
		bosstime = 0;
		bigAsteroid = 0;
		bossSpawn = false;
		for (GameObject* go : m_goList)
		{
			go->active = false;
		}
		lose = true;
		m_ship->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
		m_ship->vel = Vector3(0, 0, 0);
	}
	if (m_ship->hp <= 0)
	{
		m_lives--;
		m_score = 0;
		powerTime = 0;
		timer = 0;
		Etimer = 0;
		iframes = 0;
		SMode = 0;
		AttackTimer = 0;
		globalTimer = 0;
		bosstime = 0;
		bigAsteroid = 0;
		bossSpawn = false;
		for (GameObject* go : m_goList)
		{
			go->active = false;
		}
		m_ship->hp = 5;
		m_ship->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
	}
	if (globalTimer >= 60)
	{// reset
		m_lives = 3;
		powerTime = 0;
		timer = 0;
		Etimer = 0;
		iframes = 0;
		SMode = 0;
		AttackTimer = 0;
		globalTimer = 0;
		bosstime = 0;
		bigAsteroid = 0;
		bossSpawn = false;
		for (GameObject* go : m_goList)
		{
			go->active = false;
		}
		win = true;
		m_ship->pos = Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
		m_ship->vel = Vector3(0, 0, 0);
	}
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	//float angle = atan2(m_ship->vel.y, m_ship->vel.x);
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(m_ship->dir.y,m_ship->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], true);
		modelStack.PopMatrix();
		break;
		//Exercise 17a: render a ship texture or 3D ship model
		//Exercise 17b:	re-orientate the ship with velocity
	case GameObject::GO_ENEMY:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ENEMY], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOSS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOSS], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BIGASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		//Exercise 4b: render a bullet sphere with scale 0.2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_EBULLET:
		//Exercise 4b: render a bullet sphere with scale 0.2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_EBULLET], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BBULLET:
		//Exercise 4b: render a bullet sphere with scale 0.2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->dir.y, go->dir.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BBULLET], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP1:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP1], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP2:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP2], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP3:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP3], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP4:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP4], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP5:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP5], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAsteroid::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	RenderGO(m_ship); // Renders ship
	// Background
	modelStack.PushMatrix();
	modelStack.Scale(20, 20, 1);
	RenderMesh(meshList[GEO_BACKGROUND], true);
	modelStack.PopMatrix();

	if (!GameStart)
	{
		RenderMeshOnScreen(meshList[GEO_MENU], 40, 30, 5, 5);
	}
	if (win)
	{
		RenderMeshOnScreen(meshList[GEO_WIN], 40, 30, 5, 5);
	}
	if (lose)
	{
		RenderMeshOnScreen(meshList[GEO_LOSE], 40, 30, 5, 5);
	}
	//On screen text
	std::ostringstream ss;

	ss.str("");
	ss << "Time: " << globalTimer;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);


	ss.str("");
	ss << "Lives: " << m_lives;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	ss.str("");
	ss << "Health: " << m_ship->hp;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss << "m_score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);


	if (win)
	{
		ss.str("");
		ss << "Final Score:" << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 20, 20);
	}

	
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
