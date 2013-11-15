#include "pch.h"
#include "Enemy.h"


Enemy::Enemy()
{
	lastFired = 0.f;


}
void Enemy::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
}