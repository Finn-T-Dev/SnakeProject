#include "raylib.h"
#include "resource_dir.h"
#include <string>
#include <iostream>
#include <deque>

#define TILE_SIZE 32

// ===================================================================
// type definitions for Snake and Food
// ===================================================================
static enum directionStates {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

typedef struct Snake {
	std::deque<Rectangle> bodyDeque;	// store the snake's body as a deque
	Color color;						// color (colour) of the snake 
	directionStates direction;
	Vector2 velocity;					// speed and direction of travel
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
static int playerScore = 0;
static int frameCounter = 0;
static const int updateRate = 30;

static Snake snake;
static Food food;

// ===================================================================
// local forward declarations
// ===================================================================
// initialisation functions
static Snake initSnake();
static Food initFood();
static void initGame();

// game loop functions
static void drawGame();
static void drawSnake(const Snake& snake);
static void drawGrid();
static void getMovementDirection(Snake& snake);
static void MoveSnake(Snake& snake, bool grow);

// ===================================================================
// main entry point
// ===================================================================
int main()
{
	// tell window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create window and OpenGL context
	InitWindow(windowWidth, windowHeight, "Snake Game");

	//// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	//SearchAndSetResourceDir("resources");

	int num_rows = windowHeight / TILE_SIZE;
	int num_cols = windowWidth / TILE_SIZE;

	int currentFps = 60;
	SetTargetFPS(currentFps);

	initGame();

	// game loop
	while (!WindowShouldClose())
	{
		if (!bIsGameOver)
		{
			// toggle pause
			if (GetKeyPressed() == KEY_P)
				bIsPaused = !bIsPaused;

			if (!bIsPaused)
			{
				frameCounter ++;
				getMovementDirection(snake);
				if (frameCounter >= updateRate)
				{
					frameCounter = 0;
					std::cout << "updating snake position" << std::endl;
					// move the snake
					MoveSnake(snake, false);

					// check for collision with wall -> if true, game over
					// check for collision with own body -> if true, game over
					// check for collision with fruit -> if true, increase score and add segment to body

				}
				drawGame();
			}
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

static void getMovementDirection(Snake& snake) {
	if (IsKeyPressed(KEY_UP))
	{
		std::cout << "INPUT::KEY_UP" << std::endl;
		snake.direction = UP;
	}
	else if (IsKeyPressed(KEY_DOWN))
	{
		std::cout << "INPUT::KEY_DOWN" << std::endl;
		snake.direction = DOWN;
	}
	else if (IsKeyPressed(KEY_LEFT))
	{
		std::cout << "INPUT::KEY_LEFT" << std::endl;
		snake.direction = LEFT;
	}
	else if (IsKeyPressed(KEY_RIGHT))
	{
		std::cout << "INPUT::KEY_RIGHT" << std::endl;
		snake.direction = RIGHT;
	}
}

static void drawGame() {
	BeginDrawing();
	ClearBackground(BLACK);

	drawGrid();
	drawSnake(snake);
}

static void MoveSnake(Snake& snake, bool grow = false) {
	switch (snake.direction)
	{
	case UP:
		snake.velocity = { 0, -TILE_SIZE };
		break;

	case DOWN:
		snake.velocity = { 0, TILE_SIZE };
		break;

	case LEFT:
		snake.velocity = { -TILE_SIZE, 0 };
		break;

	case RIGHT:
		snake.velocity = { TILE_SIZE, 0 };
		break;
	}
	// Copy current head
	Rectangle newHead = snake.bodyDeque.front();

	// Move head by speed
	newHead.x += snake.velocity.x;
	newHead.y += snake.velocity.y;

	// Add new head to front
	snake.bodyDeque.push_front(newHead);

	// Remove tail unless we're growing
	if (!grow) {
		snake.bodyDeque.pop_back();
	}
}

void drawSnake(const Snake& snake) {
	for (const auto& segment : snake.bodyDeque) {
		DrawRectangleRec(segment, snake.color);
	}
}

void drawGrid()
{
	int squareSize = TILE_SIZE;
	for (int i = 0; i < GetScreenWidth() / 16; i++)
	{
		for (int j = 0; j < GetScreenHeight() / 10; j++)
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

static void initGame() {
	std::cout << "Initialising game" << std::endl;
	snake = initSnake();
	food = initFood();
}

static Snake initSnake() {
	std::cout << "Initialising snake" << std::endl;
	Snake s;
	s.color = YELLOW;
	s.velocity = { TILE_SIZE, 0 }; // start moving right
	Rectangle head = { 5 * TILE_SIZE, 5 * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	s.bodyDeque.push_back(head);
	return s;
}

static Food initFood() {
	std::cout << "Initialising food" << std::endl;
	Food f;
	f.size = { TILE_SIZE, TILE_SIZE };
	f.position = { 10 * TILE_SIZE, 10 * TILE_SIZE };
	f.active = true;
	f.color = RED;
	return f;
}