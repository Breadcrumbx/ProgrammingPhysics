
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	dir(1, 0, 0),
	momentOfInertia(1.f),
	angularVelocity(0.f),
	Stimer(0),
	Btimer(0),
	split(false),
	color()
{

}

GameObject::~GameObject()
{
}