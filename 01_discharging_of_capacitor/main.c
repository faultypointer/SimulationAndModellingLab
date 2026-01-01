#include "raylib.h"

int main()
{
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Discharging of Capacitor");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("Imagine a simulation here", 190, 150, 40, BLACK);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
