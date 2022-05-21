#pragma once

#include <iostream>
#include <SDL.h>
#include "SdlUtils.h"
#include "settings.h"

extern SDL_Renderer *renderer;

extern int mx, my;


class Rect
{
private:
	SDL_Texture *emptyTexture;
	SDL_Texture *filledTexture;
	SDL_Texture *texture;
public:
	bool isFilled;
	int xPos, yPos;
	Rect(int xPos, int yPos)
	{
		this->xPos = xPos * tileSize * sizeInc;
		this->yPos = yPos * tileSize * sizeInc;
		emptyTexture = LoadTexture("./empty.png", renderer);
		filledTexture = LoadTexture("./filled.png", renderer);
		texture = emptyTexture;
	}

	void SetFilled()
	{
		texture = filledTexture;
		isFilled = true;
	}
	void SetEmpty()
	{
		texture = emptyTexture;
		isFilled = false;
	}

	bool MouseIsInRect()
	{
		return (mx >= xPos && mx <= xPos + tileSize * sizeInc && my >= yPos && my <= yPos + tileSize * sizeInc);
	}

	void Render(int sizeInc_ = sizeInc)
	{
		RenderTexture(texture, renderer, xPos, yPos, sizeInc_);
	}
};