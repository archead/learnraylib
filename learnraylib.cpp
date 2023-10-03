#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque) {
	for (Vector2 cell : deque) {
		if (Vector2Equals(cell, element)) {
			return true;
		}
	}
	return false;
}

class Snake {
public:
	std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
	Vector2 direction = {1, 0};
	void Draw() {
		for (unsigned int i = 0; i < body.size(); i++) {
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ x * cellCount, y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}

	void Update() {
		body.pop_back();
		body.push_front(Vector2Add(body[0], direction));
	}
};

class Food {
public:

	Vector2 position;
	Texture2D texture;

	Food(std::deque<Vector2> snakeBody) {
		Image image = LoadImage("food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food() {
		UnloadTexture(texture);
	}

	void Draw() {
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}
	
	Vector2 GenerateRandomCell() {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody) {

		while(ElementInDeque(position, snakeBody)) {
			position = GenerateRandomCell();
		}
		return position;
	}
};

class Game {
public:
	Snake snake = Snake();
	Food food = Food(snake.body);

	void Draw() {
		food.Draw();
		snake.Draw();
	}

	void Update() {
		snake.Update();
		CheckCollisionWithFood();
	}

	void CheckCollisionWithFood() {
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
		}
	}
};

int main(){

	InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake");
	SetTargetFPS(60);

	Game game = Game();

	while (WindowShouldClose() == false) {
		BeginDrawing();

		if(eventTriggered(0.2)){
			game.Update();
		}

		if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1) {
			game.snake.direction = { 0, -1 };
		}
		if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1) {
			game.snake.direction = { -1, 0 };
		}
		if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1) {
			game.snake.direction = { 0, 1 };
		}
		if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1) {
			game.snake.direction = { 1, 0 };
		}

		ClearBackground(green);
		game.Draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}