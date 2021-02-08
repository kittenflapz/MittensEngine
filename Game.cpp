//
// Game.cpp
//


#include "pch.h"
#include "Game.h"


using Microsoft::WRL::ComPtr;

Game::Game() noexcept 
{

}

Game::~Game()
{
    
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    world.Initialize(window, width, height);
    
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    world.update(timer.GetTotalTicks());

    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }
    world.Render();
}

// Helper method to prepare the command list for rendering and clear the back buffers.
void Game::Clear()
{
    world.Clear();
}

// Submits the command list to the GPU and presents the back buffer contents to the screen.
void Game::Present()
{
    world.Present();
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
  /*  m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();*/

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

