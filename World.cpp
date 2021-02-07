#include "pch.h"
#include <World.h>



// constructor needs to end up like this

//World::World(HWND window)
//	: mWindow(window)
//	, mWorldView(window.getDefaultView())
//	, mTextures()
//	, mSceneGraph()
//	, mSceneLayers()
//	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
//	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
//	, mScrollSpeed(-50.f)
//	, mPlayerAircraft(nullptr)


World::World()
	: mWindow()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, 600.f, 2000.f)
	, mSpawnPosition(300.f, 1000.f) // todo: not this
	, mBackgroundScrollSpeed(-50.f)
	, mPlayerAircraft()
{
	loadTextures();
	buildScene();

	// Prepare the view
	//mWorldView.setCenter(mSpawnPosition);
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
}

void World::draw()
{
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
