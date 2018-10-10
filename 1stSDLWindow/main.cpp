#include "SDL.h"
// C standard io, prints errors to console
#include "stdio.h"
#include <iostream>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Variables
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
// Surface to show just before quitting
SDL_Surface* gQuitting = NULL;
// Array of surfaces associated with key presses
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface = NULL;

// Method declarations
SDL_Surface* loadSurface(std::string path);
bool init();
bool loadMedia();
void close();

// Loads surface from given path
SDL_Surface* loadSurface(std::string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Couldn't load image %s, SDL_Error: %s\n", path.c_str(),  SDL_GetError());
	}

	return loadedSurface;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// %s = output string from next argument
		printf("SDL couldn't initialize, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// SDL_WINDOW_SHOWN shows the window after it is created
		gWindow = SDL_CreateWindow("My first window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Couldn't create window, SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Get the window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	gQuitting = loadSurface("quitting.bmp");
	if (gQuitting == NULL)
	{
		printf("Couldn't load 'Quitting' image, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = SDL_LoadBMP("hello_world.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Couldn't load 'default' image, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = SDL_LoadBMP("up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Couldn't load 'up' image, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = SDL_LoadBMP("down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Couldn't load 'down' image, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = SDL_LoadBMP("left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Couldn't load 'left' image, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = SDL_LoadBMP("right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Couldn't load 'right' image, SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	// Deallocate surface
	SDL_FreeSurface(gCurrentSurface);
	// Pointers should point to NULL when not pointing to anything.
	gCurrentSurface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

// SDL requires a main of this format
int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media\n");
		}
		else
		{

			bool quit = false;

			// Event handler
			SDL_Event e;

			// While app is running
			while (!quit)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					// SDL_QUIT event is when user closes window
					if (e.type == SDL_QUIT)
					{

						// Blit the 'Quitting' image to the window
						SDL_BlitSurface(gQuitting, NULL, gScreenSurface, NULL);

						SDL_UpdateWindowSurface(gWindow);

						SDL_Delay(2000);

						quit = true;
					}
					// SDL_KEYDOWN when user presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
							case SDLK_UP:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
								break;
							case SDLK_DOWN:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
								break;
							case SDLK_LEFT:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
								break;
							case SDLK_RIGHT:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
								break;
							default:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
					else
					{
						gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					}

				}

				// 'Blitting' takes a source surface and stamps a copy of it onto the destination surface.
				// Blit loaded surface onto the screen surface
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				// Update the surface so we can see what was drawn to it
				SDL_UpdateWindowSurface(gWindow);

				
			}
			
		}
	}

	close();

	return 0;
}