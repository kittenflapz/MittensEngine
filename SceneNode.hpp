#pragma once
#include <vector>
#include <memory>

class SceneNode 
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	SceneNode();

	void	attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

	void	update(float deltaTime);

	DirectX::SimpleMath::Vector2 getWorldPosition() const;
	DirectX::SimpleMath::Vector2 getWorldTransform() const;


private:
	virtual void	updateMe(float deltaTime);
	void	updateChildren(float deltaTime);

	virtual void draw() const;
	virtual void drawMe() const;
	void	drawChildren() const;


private:
	std::vector<Ptr> mChildren;
	SceneNode* mParent;
};