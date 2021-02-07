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
	void draw() const override;

private:
	// maybe needs window handle passed in
	virtual void drawCurrent() const;

private:
//	sf::Sprite	 mSprite;
	char* spritePath;
};