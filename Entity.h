#pragma once

#pragma once
#include <SceneNode.hpp>
#include "pch.h"


class Entity : public SceneNode

{
public:
	void	setVelocity(DirectX::SimpleMath::Vector2 velocity);
	void	setVelocity(float vx, float vy);
	DirectX::SimpleMath::Vector2 getVelocity() const;

	virtual void updateCurrent(float deltaTime);

public:
	DirectX::SimpleMath::Vector2 mVelocity;
};
