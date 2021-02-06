#pragma once

#include <Entity.h>
#include "pch.h"

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


private:
	// maybe needs window handle passed in
	virtual void drawCurrent() const;


private:
//	sf::Sprite	 mSprite;

	char* spritePath;
};