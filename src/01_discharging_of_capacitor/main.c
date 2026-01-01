#include <stdio.h>
#include "../include/sam.h"
#include "../../external/raylib-5.5/src/raylib.h"
#include <stdlib.h>

// Raylib Stuff
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GRAPH_X 30
#define GRAPH_Y 30
#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 600

// Rk2 stuff
#define ITERATION 100
#define STEP 10
#define R 10
#define C 10
#define Q_NOT 100

int time_to_graphX(float t)
{
	return (int)((float)GRAPH_X +
		     (float)GRAPH_WIDTH * t / (float)(STEP * ITERATION));
}

int charge_to_graphY(float q)
{
	return (int)((float)GRAPH_Y + (float)GRAPH_HEIGHT * (1.f - q / Q_NOT));
}

float discharge(float tn, float qn);

int main()
{
	float *tns = (float *)malloc(sizeof(float) * (ITERATION + 1));
	float *qns = (float *)malloc(sizeof(float) * (ITERATION + 1));
	tns[0] = 0.f;
	qns[0] = Q_NOT;
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Discharging a Capacitor");
	SetTargetFPS(60);
	float tn = 0.0;
	float qn = Q_NOT;
	int iter = 1;
	Rk2 *rk2 = (Rk2 *)malloc(sizeof(Rk2));
	rk2_init_ralston(rk2, discharge, STEP);
	while (!WindowShouldClose()) {
		if (iter <= ITERATION) {
			qn = rk2_apply_update(rk2, tn, qn);
			tn += STEP;
			tns[iter] = tn;
			qns[iter] = qn;
			iter++;
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangle(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT,
			      SKYBLUE);
		for (int i = 1; i < iter; i++) {
			float x1 = time_to_graphX(tns[i - 1]);
			float x2 = time_to_graphX(tns[i]);
			float y1 = charge_to_graphY(qns[i - 1]);
			float y2 = charge_to_graphY(qns[i]);
			DrawLine(x1, y1, x2, y2, BLACK);
			// DrawCircle(time_to_graphX(tns[i]),
			// 	   charge_to_graphY(qns[i]), 2.f, RED);
		}

		EndDrawing();
	}
	CloseWindow();
	return 0;
}

float discharge(float tn, float qn)
{
	(void)tn;
	return -qn / (R * C);
}
