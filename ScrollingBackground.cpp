#include "pch.h"
#include "ScrollingBackground.h"


ScrollingBackground::ScrollingBackground()
{
}

void ScrollingBackground::scroll()
{
	if (mPosition.y > 500.f)
	{
		setPosition(mPosition.x, -500.f);
	}
}
