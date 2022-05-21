#include <iostream>
#include <SDL.h>
#include "SdlUtils.h"
#include "FieldRenderer.cpp"
#include "Converter.cpp"
#include "SocketUtility.cpp"

using std::cout;
using std::endl;
using std::cin;


SDL_Renderer* renderer;
SDL_Window* window;
SDL_Event e;

FieldRenderer* fieldRenderer;
SocketUtility* socketUtility;

bool mouseIsDown;
int mx, my;

void OnMouseLeft()
{
	fieldRenderer->UpdateFieldStatic();
}



bool Update()
{
	SDL_RenderClear(renderer);
	SDL_GetMouseState(&mx, &my);

	while (SDL_PollEvent(&e))
	{

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
				mouseIsDown = true;

		}
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
				mouseIsDown = false;
		}

		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_BACKSPACE:
				{
					fieldRenderer->ClearField();
					break;
				}
				case SDLK_g:
				{
					
					if (socketUtility->IsConnected())
					{
						char outPacket[24 * 24];
						Converter::ConvertVectorToBytes(fieldRenderer->GetMatrixVector(), outPacket);
						socketUtility->SendData(outPacket);
						string recievedData = socketUtility->GetData();
						cout << recievedData << endl;
					}
					else
					{
						cout << "Connection error!" << endl;
					}
					break;
				}
				case SDLK_c:
				{
					system("CLS");
					break;
				}
			}
		}


		if (e.type == SDL_QUIT)
			return false;
	}

	if (mouseIsDown)
		OnMouseLeft();


	if (!fieldRenderer->staticDrawed)
		fieldRenderer->UpdateField();

	fieldRenderer->Render();

	SDL_RenderPresent(renderer);
	return true;
}


int main(int argc, char* argv[])
{
	window = SDL_CreateWindow("Kurs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	socketUtility = new SocketUtility("192.168.1.102", 24  * 24);
	fieldRenderer = new FieldRenderer(SCREEN_WIDTH / tileSize / 2, SCREEN_HEIGHT / tileSize / 2);

	while (Update());

	return 0;
}
