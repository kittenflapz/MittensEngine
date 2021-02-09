#include "pch.h"
#include <World.h>



extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;


using Microsoft::WRL::ComPtr;


World::World()
	: mWindow()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, 600.f, 2000.f)
	, mSpawnPosition(300.f, 1000.f) // todo: not this
	, mBackgroundScrollSpeed(-50.f)
	, mPlayerAircraft(),
	m_featureLevel(D3D_FEATURE_LEVEL_11_0),
	m_backBufferIndex(0),
	m_rtvDescriptorSize(0),
	m_fenceValues{}
{
	loadTextures();
	buildScene();

	// Prepare the view
	//mWorldView.setCenter(mSpawnPosition);
}

World::~World()
{
	// Ensure that the GPU is no longer referencing resources that are about to be destroyed.
	WaitForGpu();
}



void World::update(float deltaTime)
{
    // Scroll the world
    //mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

    // Move the player sidewards (plane scouts follow the main aircraft)
    DirectX::SimpleMath::Vector2 position = mPlayerAircraft.getPosition();
    DirectX::SimpleMath::Vector2 velocity = mPlayerAircraft.getVelocity();

    // If player touches borders, flip its X velocity
    if (position.x <= 0
        || position.x >= 800.f)
    {
        velocity.x = -velocity.x;
        mPlayerAircraft.setVelocity(velocity);
    }

    // Apply movements
    mSceneGraph.update(deltaTime);

    mPlayerAircraft.updateCurrent(deltaTime);
    mPlayerAircraft.bounce();

    backgroundA.updateCurrent(deltaTime); 
    backgroundA.scroll();
    backgroundB.updateCurrent(deltaTime);
    backgroundB.scroll();

}

void World::draw()
{
    Clear();

	//mWindow.setView(mWorldView);
	//mWindow.draw(mSceneGraph);

	// later: iterate through scene graph drawing each item

	// now:

	//mPlayerAircraft->draw();

}

void World::loadTextures()
{
	/*mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
	mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
	mTextures.load(Textures::Desert, "Media/Textures/Desert.png");*/
}

void World::buildScene()
{
	//// Initialize the different layers
	//for (std::size_t i = 0; i < LayerCount; ++i)
	//{
	//	SceneNode::Ptr layer(new SceneNode());
	//	mSceneLayers[i] = layer.get();

	//	mSceneGraph.attachChild(std::move(layer));
	//}

	//// Prepare the tiled background
	//sf::Texture& texture = mTextures.get(Textures::Desert);
	//sf::IntRect textureRect(mWorldBounds);
	//texture.setRepeated(true);

	//// Add the background sprite to the scene
	//std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	//backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	//mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	mPlayerAircraft.setPosition(400.f, 300.f);
	mPlayerAircraft.setVelocity(5.f, 0.f);

    backgroundA.setPosition(-200.f, 250.f);
    backgroundB.setPosition(-200.f, -250.f);
    backgroundA.setVelocity(0.f, 3.f);
    backgroundB.setVelocity(0.f, 3.f);

	//// Add player's aircraft
	//std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
	//mPlayerAircraft = leader.get();
	//mPlayerAircraft->setPosition(mSpawnPosition);
	//mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
	//mSceneLayers[Air]->attachChild(std::move(leader));

	//// Add two escorting aircrafts, placed relatively to the main plane
	//std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, mTextures));
	//leftEscort->setPosition(-80.f, 50.f);
	//mPlayerAircraft->attachChild(std::move(leftEscort));

	//std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, mTextures));
	//rightEscort->setPosition(80.f, 50.f);
	//mPlayerAircraft->attachChild(std::move(rightEscort));
}

void World::Initialize(HWND window, int width, int height)
{

 m_window = window;
 m_outputWidth = std::max(width, 1);
 m_outputHeight = std::max(height, 1);

 CreateDevice();
 CreateResources();

 // TODO: Change the timer settings if you want something other than the default variable timestep mode.
 // e.g. for 60 FPS fixed timestep update logic, call:
 


}


// These are the resources that depend on the device.
void World::CreateDevice()
{
    DWORD dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    //
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
        {
            debugController->EnableDebugLayer();
        }

        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        }
    }
#endif

    DX::ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

    ComPtr<IDXGIAdapter1> adapter;
    GetAdapter(adapter.GetAddressOf());

    // Create the DX12 API device object.
    DX::ThrowIfFailed(D3D12CreateDevice(
        adapter.Get(),
        m_featureLevel,
        IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf())
    ));

#ifndef NDEBUG
    // Configure debug device (if active).
    ComPtr<ID3D12InfoQueue> d3dInfoQueue;
    if (SUCCEEDED(m_d3dDevice.As(&d3dInfoQueue)))
    {
#ifdef _DEBUG
        d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif
        D3D12_MESSAGE_ID hide[] =
        {
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
        };
        D3D12_INFO_QUEUE_FILTER filter = {};
        filter.DenyList.NumIDs = _countof(hide);
        filter.DenyList.pIDList = hide;
        d3dInfoQueue->AddStorageFilterEntries(&filter);
    }
#endif

    // Create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DX::ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf())));

    // Create descriptor heaps for render target views and depth stencil views.
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
    rtvDescriptorHeapDesc.NumDescriptors = c_swapBufferCount;
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
    dsvDescriptorHeapDesc.NumDescriptors = 1;
    dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(m_rtvDescriptorHeap.ReleaseAndGetAddressOf())));
    DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(m_dsvDescriptorHeap.ReleaseAndGetAddressOf())));

    m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create a command allocator for each back buffer that will be rendered to.
    for (UINT n = 0; n < c_swapBufferCount; n++)
    {
        DX::ThrowIfFailed(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocators[n].ReleaseAndGetAddressOf())));
    }

    // Create a command list for recording graphics commands.
    DX::ThrowIfFailed(m_d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(m_commandList.ReleaseAndGetAddressOf())));
    DX::ThrowIfFailed(m_commandList->Close());

    // Create a fence for tracking GPU execution progress.
    DX::ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_backBufferIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));
    m_fenceValues[m_backBufferIndex]++;

    m_fenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
    if (!m_fenceEvent.IsValid())
    {
        throw std::exception("CreateEvent");
    }

    // TODO: Initialize device dependent objects here (independent of window size).

    m_graphicsMemory = std::make_unique<GraphicsMemory>(m_d3dDevice.Get());

    m_resourceDescriptors = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(),
        Descriptors::Count);

    ResourceUploadBatch resourceUpload(m_d3dDevice.Get());

    resourceUpload.Begin();

    DX::ThrowIfFailed(
        CreateWICTextureFromFile(m_d3dDevice.Get(), resourceUpload, L"Desert.png",
            m_background.ReleaseAndGetAddressOf()));

    CreateShaderResourceView(m_d3dDevice.Get(), m_background.Get(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Background));

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(m_d3dDevice.Get(), resourceUpload, L"Eagle.dds",
            m_texture.ReleaseAndGetAddressOf()));
    RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

    SpriteBatchPipelineStateDescription pd(rtState);
    m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dDevice.Get(), resourceUpload, pd);

    XMUINT2 airplaneSize = GetTextureSize(m_texture.Get());

    m_origin.x = float(airplaneSize.x / 2);
    m_origin.y = float(airplaneSize.y / 2);

    CreateShaderResourceView(m_d3dDevice.Get(), m_texture.Get(),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Airplane));

    auto uploadResourcesFinished = resourceUpload.End(m_commandQueue.Get());

    uploadResourcesFinished.wait();

}

// Allocate all memory resources that change on a window SizeChanged event.
void World::CreateResources()
{
    // Wait until all previous GPU work is complete.
    WaitForGpu();

    // Release resources that are tied to the swap chain and update fence values.
    for (UINT n = 0; n < c_swapBufferCount; n++)
    {
        m_renderTargets[n].Reset();
        m_fenceValues[n] = m_fenceValues[m_backBufferIndex];
    }

    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
    const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(c_swapBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = c_swapBufferCount;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a swap chain for the window.
        ComPtr<IDXGISwapChain1> swapChain;
        DX::ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
            m_commandQueue.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            swapChain.GetAddressOf()
        ));

        DX::ThrowIfFailed(swapChain.As(&m_swapChain));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        DX::ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the back buffers for this window which will be the final render targets
    // and create render target views for each of them.
    for (UINT n = 0; n < c_swapBufferCount; n++)
    {
        DX::ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(m_renderTargets[n].GetAddressOf())));

        wchar_t name[25] = {};
        swprintf_s(name, L"Render target %u", n);
        m_renderTargets[n]->SetName(name);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(
            m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            static_cast<INT>(n), m_rtvDescriptorSize);
        m_d3dDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvDescriptor);
    }

    // Reset the index to the current back buffer.
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view
    // on this surface.
    CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        depthBufferFormat,
        backBufferWidth,
        backBufferHeight,
        1, // This depth stencil view has only one texture.
        1  // Use a single mipmap level.
    );
    depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
    depthOptimizedClearValue.Format = depthBufferFormat;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    DX::ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
        &depthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        IID_PPV_ARGS(m_depthStencil.ReleaseAndGetAddressOf())
    ));

    m_depthStencil->SetName(L"Depth stencil");

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthBufferFormat;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

    m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &dsvDesc, m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // TODO: Initialize windows-size dependent objects here.

    D3D12_VIEWPORT viewport = { 0.0f, 0.0f,
    static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight),
    D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
    m_spriteBatch->SetViewport(viewport);

    m_screenPos.x = backBufferWidth / 2.f;
    m_screenPos.y = backBufferHeight / 2.f;
    //m_spriteBatch->SetRotation(DXGI_MODE_ROTATION_UNSPECIFIED);


    m_fullscreenRect = { 0, 0, static_cast<LONG>(backBufferWidth), static_cast<LONG>(backBufferHeight) };


}

void World::WaitForGpu() noexcept
{
    if (m_commandQueue && m_fence && m_fenceEvent.IsValid())
    {
        // Schedule a Signal command in the GPU queue.
        UINT64 fenceValue = m_fenceValues[m_backBufferIndex];
        if (SUCCEEDED(m_commandQueue->Signal(m_fence.Get(), fenceValue)))
        {
            // Wait until the Signal has been processed.
            if (SUCCEEDED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent.Get())))
            {
                WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);

                // Increment the fence value for the current frame.
                m_fenceValues[m_backBufferIndex]++;
            }
        }
    }
}

void World::MoveToNextFrame()
{
    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = m_fenceValues[m_backBufferIndex];
    DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

    // Update the back buffer index.
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_fence->GetCompletedValue() < m_fenceValues[m_backBufferIndex])
    {
        DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_backBufferIndex], m_fenceEvent.Get()));
        WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    m_fenceValues[m_backBufferIndex] = currentFenceValue + 1;
}

// This method acquires the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, try WARP. Otherwise throw an exception.
void World::GetAdapter(IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;
    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf()); ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        DX::ThrowIfFailed(adapter->GetDesc1(&desc));

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // Don't select the Basic Render Driver adapter.
            continue;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), m_featureLevel, _uuidof(ID3D12Device), nullptr)))
        {
            break;
        }
    }

#if !defined(NDEBUG)
    if (!adapter)
    {
        if (FAILED(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))))
        {
            throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
        }
    }
#endif

    if (!adapter)
    {
        throw std::exception("No Direct3D 12 device found");
    }

    *ppAdapter = adapter.Detach();
}

void World::OnDeviceLost()
{
    // TODO: Perform Direct3D resource cleanup.

    for (UINT n = 0; n < c_swapBufferCount; n++)
    {
        m_commandAllocators[n].Reset();
        m_renderTargets[n].Reset();
    }

    m_depthStencil.Reset();
    m_fence.Reset();
    m_commandList.Reset();
    m_swapChain.Reset();
    m_rtvDescriptorHeap.Reset();
    m_dsvDescriptorHeap.Reset();
    m_commandQueue.Reset();
    m_d3dDevice.Reset();
    m_dxgiFactory.Reset();

    CreateDevice();
    CreateResources();
    m_graphicsMemory.reset();


    // sprite rendering stuff
    m_texture.Reset();
    m_resourceDescriptors.reset();

    m_spriteBatch.reset();

    m_background.Reset();
}

void World::Render()
{
 
    // Prepare the command list to render a new frame.
    Clear();

    // TODO: Add your rendering code here.

    ////** TO JUST DRAW A SPRITE: **//

    //ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    //m_commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    //m_spriteBatch->Begin(m_commandList.Get());

    //m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
    //    GetTextureSize(m_texture.Get()),
    //    m_screenPos, nullptr, Colors::White, 0.f, m_origin);

    //m_spriteBatch->End();


    ////** TO ROTATE A SPRITE: **//
    //float time = float(m_timer.GetTotalSeconds());

    //ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    //m_commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    //m_spriteBatch->Begin(m_commandList.Get());

    //m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Airplane),
    //    GetTextureSize(m_texture.Get()),
    //    m_screenPos, nullptr, Colors::White, cosf(time) * 4.f, m_origin);

    //m_spriteBatch->End();

    //**TO SCALE A SPRITE**//

    //float time = float(m_timer.GetTotalSeconds());

    //ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    //m_commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    //m_spriteBatch->Begin(m_commandList.Get());

    //m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
    //    GetTextureSize(m_texture.Get()),
    //    m_screenPos, nullptr, Colors::White, 0.f, m_origin, cosf(time) + 2.f);

    //m_spriteBatch->End();

    //** TO KILL A MOCKINGBIRD **//
    // jk

    // ** TO TINT A SPRITE **//

    //ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    //m_commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    //m_spriteBatch->Begin(m_commandList.Get());

    //m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Cat),
    //    GetTextureSize(m_texture.Get()),
    //    m_screenPos, nullptr, Colors::Green, 0.f, m_origin);

    //m_spriteBatch->End();


    // SPRITE AND BACKGROUND

    ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap() };
    m_commandList->SetDescriptorHeaps(static_cast<UINT>(std::size(heaps)), heaps);

    m_spriteBatch->Begin(m_commandList.Get());

    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Background),
        GetTextureSize(m_background.Get()),
        backgroundA.getPosition(), nullptr, Colors::White, 0.f, m_origin, 2.f);
    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Background),
        GetTextureSize(m_background.Get()),
        backgroundB.getPosition(), nullptr, Colors::White, 0.f, m_origin, 2.f);

    m_spriteBatch->Draw(m_resourceDescriptors->GetGpuHandle(Descriptors::Airplane),
        GetTextureSize(m_texture.Get()),
        mPlayerAircraft.getPosition(), nullptr, Colors::White, 0.f, m_origin);


    m_spriteBatch->End();

    // Show the new frame.
    Present();
    m_graphicsMemory->Commit(m_commandQueue.Get());
}

// Helper method to prepare the command list for rendering and clear the back buffers.
void World::Clear()
{
    // Reset command list and allocator.
    DX::ThrowIfFailed(m_commandAllocators[m_backBufferIndex]->Reset());
    DX::ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_backBufferIndex].Get(), nullptr));

    // Transition the render target into the correct state to allow for drawing into it.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &barrier);

    // Clear the views.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(
        m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        static_cast<INT>(m_backBufferIndex), m_rtvDescriptorSize);
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDescriptor(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    m_commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    m_commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    m_commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
    D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(m_outputWidth), static_cast<LONG>(m_outputHeight) };
    m_commandList->RSSetViewports(1, &viewport);
    m_commandList->RSSetScissorRects(1, &scissorRect);
}

// Submits the command list to the GPU and presents the back buffer contents to the screen.
void World::Present()
{
    // Transition the render target to the state that allows it to be presented to the display.
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &barrier);

    // Send the command list off to the GPU for processing.
    DX::ThrowIfFailed(m_commandList->Close());
    m_commandQueue->ExecuteCommandLists(1, CommandListCast(m_commandList.GetAddressOf()));

    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);

        MoveToNextFrame();
    }
}



