#include "pch.h"
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

void Entity::setPosition(DirectX::SimpleMath::Vector2 position)
{
	mPosition = position;
}

void Entity::setPosition(float px, float py)
{
	mPosition.x = px;
	mPosition.y = py;
}

DirectX::SimpleMath::Vector2 Entity::getPosition() const
{
	return mPosition;
}

void Entity::updateCurrent(float deltaTime)
{
	// todo: wat
	 move(mVelocity);
}

void Entity::move(DirectX::SimpleMath::Vector2 moveDelta)
{
	mPosition += moveDelta;
}


