#include "raylib.h"
#include "resource_dir.h"

int main()
{
	// tell window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create window and OpenGL context
	InitWindow(1280, 800, "Snake Game");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture rabbit = LoadTexture("wabbit_alpha.png");

	int currentMonitor = GetCurrentMonitor();

	int refreshRate = GetMonitorRefreshRate(currentMonitor);

	Vector2 textPos = { 0.0f, 0.0f };
	Vector2 rabbitPos = { 0.0f, 0.0f };

	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);
 
		DrawText("Hello", textPos.x, textPos.y, 20, WHITE);
		textPos.x += int(1);
		textPos.y += int(1);
		if (textPos.x >= 1280)
		{
			textPos.x = 0;
		}
		if (textPos.y >= 800)
		{
			textPos.y = 0;
		}
		rabbitPos.x = 1280 - textPos.x;
		rabbitPos.y = 800 - textPos.y;
		DrawTexture(rabbit, rabbitPos.x, rabbitPos.y, WHITE);

		

		EndDrawing();
	}


	UnloadTexture(rabbit);
	CloseWindow();
	return 0;
}

void Move()
{

}