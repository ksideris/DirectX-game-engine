#include "pch.h"

#include "GamePlayElement.h"

void GamePlayElement::Update(float timeDelta)
{
	GameObject::Update(timeDelta);
}

float GamePlayElement::GetImpactSize()
{
	return 0;
}
ImpactResult GamePlayElement::ProcessHit(float ImpactFactor)
{
	_dead = true;

	if (type == GamePlayType::RING)
		return ImpactResult::score;
	else if (type == GamePlayType::FINISHLINE)
	{
		_dead = false;
		return ImpactResult::finish;

	}

	return ImpactResult::death;
}