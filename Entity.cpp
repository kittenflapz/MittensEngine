#include "Entity.h"

void Entity::setVelocity(DirectX::SimpleMath::Vector2 velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

DirectX::SimpleMath::Vector2 Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::updateCurrent(float deltaTime)
{

	// todo: wat
	// move(mVelocity * dt.asSeconds());
}