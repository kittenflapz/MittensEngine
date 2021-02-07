#pragma once
#include <SceneNode.hpp>


class Entity : public SceneNode

{
public:
	void	setVelocity(DirectX::SimpleMath::Vector2 velocity);
	void	setVelocity(float vx, float vy);
	DirectX::SimpleMath::Vector2 getVelocity() const;

	void	setPosition(DirectX::SimpleMath::Vector2 position);
	void	setPosition(float px, float py);
	DirectX::SimpleMath::Vector2 getPosition() const;

	virtual void updateCurrent(float deltaTime);


public:
	DirectX::SimpleMath::Vector2 mVelocity;
	DirectX::SimpleMath::Vector2 mPosition;
};
