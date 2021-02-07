#pragma once

#include <SceneNode.hpp>
#include <Aircraft.h>
#include <array>


// make singleton maybe>? ? ?? 
class World
{
public:
	World();
	void	update(float deltaTime);
	void	draw();

	Aircraft mPlayerAircraft;

private:
	void	loadTextures();
	void	buildScene();

// TODO: this, later

//private:
//	enum Layer
//	{
//		Background,
//		Air,
//		LayerCount
//	};

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

};

#pragma endregion