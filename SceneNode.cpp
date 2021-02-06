#include <SceneNode.hpp>
#include <algorithm>
#include <cassert>


SceneNode::SceneNode()
	: mChildren()
	, mParent(nullptr)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(float deltaTime)
{
	updateMe(deltaTime);
	updateChildren(deltaTime);
}

void SceneNode::updateMe(float deltaTime)
{
	// stuff pertaining to this node's update
}

void SceneNode::updateChildren(float deltaTime)
{
	for (Ptr& child : mChildren)
	{
		child->update(deltaTime);
	}
}


void SceneNode::draw() const
{
	//// Apply transform of current node
	//states.transform *= getTransform();

	//// Draw node and children with changed transform
	//drawMe(target, states);
	//drawChildren(target, states);
}

void SceneNode::drawMe() const
{
	// Do nothing by default
}

void SceneNode::drawChildren() const
{
	for (const Ptr& child : mChildren)
		child->draw();
}

DirectX::SimpleMath::Vector2 SceneNode::getWorldPosition() const
{

	// what is difference between position and transform ?? ?? ?
	return getWorldTransform();
}

DirectX::SimpleMath::Vector2 SceneNode::getWorldTransform() const
{
	DirectX::SimpleMath::Vector2 transform(0, 0);

	// traverse up through scene graph to find root transform  . . . . maybe
	//for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	//	transform = node->getTransform() * transform;

	return transform;
}