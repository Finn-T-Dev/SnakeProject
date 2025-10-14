#include "raylib.h"
#include "resource_dir.h"
#include <string>

#define TILE_SIZE 32

static void HandleUserInput(Vector2& playerPos);
static void DrawGrid();
static void SetNewFoodLocation(Rectangle& food);
static void SetPlayerScore(int playerScore);

const int windowWidth = 1280;
const int windowHeight = 800;

static bool bIsGameOver;
static bool bIsPaused;
static int playerScore;

int main()
{
	// tell window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create window and OpenGL context
	InitWindow( windowWidth, windowHeight, "Snake Game");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	int num_rows = windowHeight / TILE_SIZE;
	int num_cols = windowWidth / TILE_SIZE;

	Vector2 textPos = { 0.0f, 0.0f };

	Rectangle player = { (windowWidth / 2) - (TILE_SIZE / 2), windowHeight / 2 - (TILE_SIZE / 2), TILE_SIZE, TILE_SIZE };
	Vector2 playerPos = { player.x, player.y };
	Rectangle food = { GetRandomValue(0, windowWidth), GetRandomValue(0, windowHeight), TILE_SIZE, TILE_SIZE };
	Vector2 foodPos = { food.x, food.y };

	int currentFps = 60;
	SetTargetFPS(currentFps);

	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);
		DrawGrid();
 
		DrawText("Hello", textPos.x, textPos.y, 20, WHITE);
		textPos.x += int(1);
		textPos.y += int(1);
		if (textPos.x >= windowWidth)
		{
			textPos.x = 0;
		}
		if (textPos.y >= windowHeight)
		{
			textPos.y = 0;
		}
		HandleUserInput(playerPos);
		
		player.x = playerPos.x;
		player.y = playerPos.y;

		DrawRectangleRec(player, DARKBLUE);
		bool bCollidedWithFood = CheckCollisionRecs(player, food);

		if (bCollidedWithFood)
		{
			SetNewFoodLocation(food);
		}

		DrawRectangleRec(food, RED);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void HandleUserInput(Vector2& playerPos)
{
	if (IsKeyDown(KEY_RIGHT)) playerPos.x += 2.0f;
	if (IsKeyDown(KEY_LEFT)) playerPos.x -= 2.0f;
	if (IsKeyDown(KEY_UP)) playerPos.y -= 2.0f;
	if (IsKeyDown(KEY_DOWN)) playerPos.y += 2.0f;
}

void SetNewFoodLocation(Rectangle& food)
{
	food.x = static_cast<float>(GetRandomValue(0, windowWidth));
	food.y = static_cast<float>(GetRandomValue(0, windowHeight));
}

void DrawGrid()
{
	int squareSize = TILE_SIZE;
	for (int i = 0; i < GetScreenWidth()/16; i++)
	{
		for (int j = 0; j < GetScreenHeight()/10; j++)
		{
			if (i % 2 == 0)
			{
				if (j % 2 == 0)
				{
					DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, GREEN);
				}
				else {
					DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, DARKGREEN);
				}
			}
			else
			{
				if (j % 2 == 0)
				{
					DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, DARKGREEN);
				}
				else {
					DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, GREEN);
				}
			}
		}
	}
}