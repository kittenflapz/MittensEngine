#pragma once

#include <SceneNode.hpp>
#include <Aircraft.h>
#include <array>


// make singleton maybe>? ? ?? 
class World
{
public:
	World();
    ~World();
	void	update(float deltaTime);
	void	draw();

	Aircraft mPlayerAircraft;

    public:
	void	loadTextures();
	void	buildScene();

    void Initialize(HWND window, int width, int height);

// TODO: this, later

//private:
//	enum Layer
//	{
//		Background,
//		Air,
//		LayerCount
//	};


    void CreateDevice();
    void CreateResources();

    void WaitForGpu() noexcept;
    void MoveToNextFrame();
    void GetAdapter(IDXGIAdapter1** ppAdapter);

    void OnDeviceLost();

    void Render();
    void Clear();
    void Present();

private:
	HWND mWindow;

	// texture vector here
	// TextureHolder	 mTextures;

	SceneNode	mSceneGraph;


	// count will need to change back to layer number
	std::array<SceneNode*, 1>	mSceneLayers;

	DirectX::SimpleMath::Rectangle mWorldBounds;
	DirectX::SimpleMath::Vector2 mSpawnPosition;
	float	 mBackgroundScrollSpeed;

    HWND  m_window;
    int  m_outputWidth;
    int  m_outputHeight;



	// stuff that used to be in game


    // Direct3D Objects
    D3D_FEATURE_LEVEL                                   m_featureLevel;
    static const UINT                                   c_swapBufferCount = 2;
    UINT                                                m_backBufferIndex;
    UINT                                                m_rtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
    Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
    UINT64                                              m_fenceValues[c_swapBufferCount];
    Microsoft::WRL::Wrappers::Event                     m_fenceEvent;

    // Rendering resources
    Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;




    // Sprite rendering stuff

    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;


    // one of these for each sprite
    enum Descriptors
    {
        Airplane,
        Background,
        Count
    };

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    RECT m_fullscreenRect;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_background;



};

#pragma endregion