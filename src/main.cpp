#include "raylib.h"
#include "resource_dir.h"
#include <string>
#include <iostream>
#include <deque>

#define TILE_SIZE 32

// ===================================================================
// type definitions for Snake and Food
// ===================================================================
typedef struct Snake {
	std::deque<Rectangle> bodyDeque;
	Vector2 speed;
	Color color;
} Snake;

typedef struct Food {
	Vector2 position;
	Vector2 size;
	bool active;
	Color color;
} Food;

// ===================================================================
// global variable declarations
// ===================================================================
static const int windowWidth = 1280;
static const int windowHeight = 800;

static bool bIsGameOver = false;
static bool bIsPaused = false;
static int playerScore;

static Food fruit = { 0 };
static std::deque<Snake> snake = {};

// ===================================================================
// local forward declarations
// ===================================================================
Snake initSnake(Vector2 startPos, float blockSize, Color color)
{
	Snake snake;
	snake.color = color;
	snake.speed = { blockSize, 0 };

	Rectangle head = { startPos.x, startPos.y, blockSize, blockSize };
	snake.bodyDeque.push_back(head);

	return snake;
}
static void HandleUserInput(Vector2& playerPos);
static void SetPlayerPosition(Rectangle& player, Vector2& playerPos);
static void DrawGrid();
static void SetNewFoodLocation(Rectangle& food);
static void UpdateMovingTextPos(Vector2& textPos);

// ===================================================================
// main entry point
// ===================================================================
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
	float moveDelay = 0.25f;
	float moveTimer = 0.0f;

	// game loop
	while (!WindowShouldClose())
	{
		if (!bIsPaused)
		{
			moveTimer += GetFrameTime();

			UpdateMovingTextPos(textPos);

			if (moveTimer >= moveDelay)
			{
				moveTimer = 0.0f;
				SetPlayerPosition(player, playerPos);
				player.x = playerPos.x;
				player.y = playerPos.y;
			}
			bool bCollidedWithFood = CheckCollisionRecs(player, food);

			if (bCollidedWithFood)
			{
				playerScore += 100;
				std::cout << "PLAYER SCORE:" << playerScore << std::endl;
				SetNewFoodLocation(food);
			}
		}
		BeginDrawing();
		ClearBackground(BLACK);
		DrawGrid();
		DrawText("Hello", textPos.x, textPos.y, 20, WHITE);
		HandleUserInput(playerPos);
		DrawRectangleRec(food, RED);
		DrawRectangleRec(player, DARKBLUE);
		DrawText(("Score: " + std::to_string(playerScore)).c_str(), 20, 20, 30, WHITE);

		if (bIsPaused)
		{
			DrawText("PAUSED", 640, 400, 40, WHITE);

		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void HandleUserInput(Vector2& playerPos)
{
	if (IsKeyDown(KEY_RIGHT)) playerPos.x += 5.0f;
	if (IsKeyDown(KEY_LEFT)) playerPos.x -= 5.0f;
	if (IsKeyDown(KEY_UP)) playerPos.y -= 5.0f;
	if (IsKeyDown(KEY_DOWN)) playerPos.y += 5.0f;
	if (IsKeyPressed(KEY_P)) bIsPaused = !bIsPaused;
}

static void SetPlayerPosition(Rectangle& player, Vector2& playerPos)
{
	player.x = playerPos.x;
	player.y = playerPos.y;
}

void SetNewFoodLocation(Rectangle& food)
{
	food.x = static_cast<float>(GetRandomValue(0, windowWidth));
	food.y = static_cast<float>(GetRandomValue(0, windowHeight));
}

void UpdateMovingTextPos(Vector2& textPos)
{
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