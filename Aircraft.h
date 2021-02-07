#pragma once
#include <Entity.h>

class Aircraft : public Entity
{
public:	

	// todo: multiple aircraft types
	//enum Type
	//{
	//	Eagle,
	//	Raptor,
	//};

public:
	Aircraft();
	//void draw(std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptor, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList,
	//	std::unique_ptr<DirectX::SpriteBatch> spriteBatch, Microsoft::WRL::ComPtr<ID3D12Resource> texture, DirectX::SimpleMath::Vector2 origin) const; 

	// maybe needs window handle passed in
	virtual void drawMe() const;
	void bounce();

private:
//	sf::Sprite	 mSprite;
	char* spritePath;


};