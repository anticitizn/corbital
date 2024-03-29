#define SDL_MAIN_HANDLED
#include <iostream>
#include <src/SimulationWrapper.h>

using namespace std;

bool running = true;

void processInput(SDL_Event event, SimulationWrapper& sim)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				sim.OnClick(mouseX, mouseY);
			break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					// ESC
					case 27:
						running = false;
					break;
					
					case 32: // space
						sim.Clear();
					break;
					
					case 49: // 1
						sim.SelectEntity(1);
					break;
					
					case 50: // 2
						sim.SelectEntity(2);
					break;
					
					case 51: // 3
						sim.SelectEntity(3);
					break;
					
					case 52: // 4
						sim.ChangeTimestep(5.0);
					break;
					
					case 53: // 5
						sim.ChangeTimestep(4.0);
					break;
										
					case 119: // w
						sim.camera.Pos = Vector2d(sim.camera.Pos.x(), sim.camera.Pos.y() - sim.camera.Scale);
					break;
					
					case 97: // a
						sim.camera.Pos = Vector2d(sim.camera.Pos.x() - sim.camera.Scale, sim.camera.Pos.y());
					break;
					
					case 115: // s
						sim.camera.Pos = Vector2d(sim.camera.Pos.x(), sim.camera.Pos.y() + sim.camera.Scale);
					break;
						
					case 100: // d
						sim.camera.Pos = Vector2d(sim.camera.Pos.x() + sim.camera.Scale, sim.camera.Pos.y());
					break;
				}
			break;
			
			case SDL_MOUSEWHEEL:
				if (event.wheel.y > 0)
				{
					// scroll up
					sim.camera.Scale /= 1.5;
				}
				else if (event.wheel.y < 0)
				{
					// scroll down
					sim.camera.Scale *= 1.5;
				}
			break;
		}
	}
}

int main()
{
	// turn all of this event, scale and offset stuff into a class, it's unusable this way
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));

	SimulationWrapper sim;

	while (running)
	{
		processInput(event, sim);
		sim.Tick();
		SDL_Delay(30);
	}
}
