#include "SceneKinematics.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneKinematics::SceneKinematics()
{
}

SceneKinematics::~SceneKinematics()
{
}

void SceneKinematics::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	
	m_gravity.Set(0, -9.8f, 0); //init gravity as 9.8ms-2 downwards
	Math::InitRNG();

	m_ghost = new GameObject(GameObject::GO_BALL);
	ballRadius = m_ghost->scale.x;
	//Exercise 1: construct 10 GameObject with type GO_BALL and add into m_goList
	for (int i = 0; i < 1000; i++)
	{
		GameObject* ball = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(ball);
	}
}

void SceneKinematics::Update(double dt)
{
	SceneBase::Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed(VK_OEM_PLUS))
	{
		//Exercise 6: adjust simulation speed
		m_speed += 1.f * dt;
	}
	if(Application::IsKeyPressed(VK_OEM_MINUS))
	{
		//Exercise 6: adjust simulation speed
		m_speed -= 1.f * dt;
		if (m_speed <= 0)
		{
			m_speed = 0;
		}
	}
	if(Application::IsKeyPressed('C'))
	{
		//Exercise 9: clear screen
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
				go->active = false;
		}
	}
	if(Application::IsKeyPressed('B'))
	{
		//Exercise 9: spawn balls
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_BALL;
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();
				go->pos.Set(rand() % int(m_worldWidth), rand() % int(m_worldHeight), 0);
				go->vel.Set(rand() % 200 - 100, rand() % 200 - 100, 0); //random range between -100 - 100
			}
		}
	}
	if(Application::IsKeyPressed('V'))
	{
		//Exercise 9: spawn obstacles
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_CUBE;
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();
				go->pos.Set(rand() % int(m_worldWidth), rand() % int(m_worldHeight), 0);
			}
		}
	}

	//Mouse Section
	static bool bLButtonState = false;
	//Exercise 10: ghost code here
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		//Exercise 10: spawn ghost ball
		if (!m_ghost->active)
		{
			m_ghost->active = true;
			m_ghost->pos = Vector3(x / w * m_worldWidth, (h - y) / h * m_worldHeight, 0);
		}
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
		
		//Exercise 4: spawn ball
		// x = 200/800 * 133
		// y (600-100)/600 * 100
		/*for (std::vector<GameObject*>::iterator go = m_goList.begin(); go != m_goList.end(); go++)
		{
			if (!(*go)->active)
			{
				(*go)->active = true;
				(*go)->type = GameObject::GO_BALL;
				double x, y;
				Application::GetCursorPos(&x, &y);
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();
				
				float posX = x / w * m_worldWidth;
				float posY = (h - y) / h * m_worldHeight;
				
				(*go)->pos.Set(posX, posY);
				(*go)->vel.Set(20, 20, 0);
				break;
			}
		}*/
		//Exercise 10: replace Exercise 4 code and use ghost to determine ball velocity
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = *it;
			if (!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_BALL;
				go->scale = m_ghost->scale;

				double x, y;
				Application::GetCursorPos(&x, &y);
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();

				go->pos = m_ghost->pos;
				go->vel = go->pos - Vector3(x / w * m_worldWidth, (h - y) / h * m_worldHeight, 0);
				m_ghost->vel = go->vel;
				m_ghost->active = false;
				m_timeGO = go;
				break;
			}
		}
		//Exercise 10: replace Exercise 4 code and use ghost to determine ball velocity

		//Exercise 11: kinematics equation
		//v = u + a * t
		//t = (v - u ) / a

		//v * v = u * u + 2 * a * s
		//s = - (u * u) / (2 * a)
						
		//s = u * t + 0.5 * a * t * t
		//(0.5 * a) * t * t + (u) * t + (-s) = 0
	}
	
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		//bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		//Exercise 7: spawn obstacles using GO_CUBE
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / w * m_worldWidth;
		float posY = (h - y) / h * m_worldHeight;


		for (int i = 0; i < m_goList.size(); ++i)
		{
			if (m_goList[i]->active == false)
			{
				m_goList[i]->active = true;
				m_goList[i]->type = GameObject::GO_CUBE;
				m_goList[i]->pos.Set(posX, posY);
				break;
			}
		}
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section
	fps = (float)(1.f / dt);

	//Exercise 11: update kinematics information
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			if(go->type == GameObject::GO_BALL)
			{
				//Exercise 2: implement equation 1 & 2
				go->vel += m_gravity * dt * m_speed;
				go->pos += go->vel * dt;

				//Exercise 12: replace Exercise 2 code and use average speed instead
			}

			//Exercise 8: check collision with GO_CUBE
			if (go->type == GameObject::GO_BALL)
			{
				for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					GameObject* temp = (GameObject*)*it2;
					// No point checking your own self
					if (temp == go)
						continue;
					if (temp->active && temp->type == GameObject::GO_CUBE)
					{
						// checking length between the two objects
						Vector3 dist = temp->pos - go->pos;
						float combinedRadii = go->scale.x + temp->scale.x;
						if (dist.LengthSquared() < combinedRadii * combinedRadii )
						{
							go->active = false;
							temp->active = false;
						}
					}
					else if (temp->active && temp->type == GameObject::GO_BALL)
					{
						Vector3 dist = temp->pos - go->pos;
						float combinedRadii = go->scale.x + temp->scale.x;
						float sqrDistance = (go->pos - temp->pos).LengthSquared();
						if (dist.LengthSquared() < combinedRadii * combinedRadii)
						{
							// Elastic collision
							float displacement = (go->vel - temp->vel).Dot(go->pos - temp->pos) / sqrDistance;
							float combinedMasses = go->mass + temp->mass;
							go->vel = go->vel - (2 * temp->mass / combinedMasses) * displacement * (-dist);
							temp->vel = temp->vel - (2 * go->mass / combinedMasses) * displacement * (dist);

							/*m1 = go->mass;
							m2 = go2->mass;
							u1 = go->vel;
							u2 = go2->vel;

							float sqDist = (go->pos - go2->pos).LengthSquared();
							float dp = (u1 - u2).Dot(go->pos - go2->pos) / sqDist;
							v1 = u1 - 2 * m2 / (m1 + m2) * dp * (go->pos - go2->pos);
							v2 = u2 - 2 * m1 / (m1 + m2) * dp * (go2->pos - go->pos);

							go->vel = v1;
							go2->vel = v2;*/
						} 
					}
				}
			}

			//Exercise 5: unspawn ball when outside window
			
			if (go->pos.x < ballRadius || go->pos.x > m_worldWidth - ballRadius)
			{
				go->vel.x *= -1;
			}
			if (go->pos.y > m_worldHeight - ballRadius || go->pos.y < ballRadius)
			{
				go->vel.y *= -1;
			}
			// If clip out of screen, make sure it is deleted
			if (go->pos.x < 0 - go->scale.Length() || go->pos.x > m_worldWidth + go->scale.Length() || go->pos.y < 0 - go->scale.Length() || go->pos.y > m_worldHeight + go->scale.Length())
			{
				go->active = false;
			}
		}
	}
}

void SceneKinematics::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 3: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();

		break;
	case GameObject::GO_CUBE:
		//Exercise 7: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneKinematics::Render()
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
	if(m_ghost->active)
	{
		RenderGO(m_ghost);
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
	
	//Exercise 6: print simulation speed
	ss.str("");
	ss.clear();
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	//Exercise 10: print m_ghost position and velocity information
	ss.str("");
	ss.precision(4);
	ss << "Gpos: " << m_ghost->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	ss.str("");
	ss.precision(4);
	ss << "Gvel: " << m_ghost->vel;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);
	//Exercise 11: print kinematics information

	RenderTextOnScreen(meshList[GEO_TEXT], "Kinematics", Color(0, 1, 0), 3, 0, 0);
}

void SceneKinematics::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	
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
