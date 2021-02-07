#include "pch.h"
#include "Aircraft.h"
#include "Game.h"

Aircraft::Aircraft()
{
	
}

void Aircraft::drawMe() const
{
    
}
//
//void Aircraft::draw(std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptor, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList,
//    std::unique_ptr<DirectX::SpriteBatch> spriteBatch, Microsoft::WRL::ComPtr<ID3D12Resource> texture, DirectX::SimpleMath::Vector2 origin) const;
//{
//	//drawMe();
//}

void Aircraft::bounce()
{
	if (mPosition.x > 750.f || mPosition.x < 50.f)
	{
		mVelocity = -mVelocity;
	}
}