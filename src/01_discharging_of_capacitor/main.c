#include "../include/sam.h"
#include "../../external/raylib-5.5/src/raylib.h"
#include <stdio.h>
#include <stdlib.h>

// Raylib Stuff
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GRAPH_X 30
#define GRAPH_Y 30
#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 600

#define TABLE_X 660
#define TABLE_Y 30
#define TABLE_WIDTH 500
#define TABLE_HEIGHT 600
#define TABLE_NUM_ROWS 20
#define TABLE_TEXT_OFFSET_X 50
#define CELL_WIDTH TABLE_HEIGHT / TABLE_NUM_ROWS

// Rk2 stuff
#define ITERATION 20
#define STEP 10
#define R 10
#define C 10
#define Q_NOT 100

float time_to_graphX(float t)
{
	return GRAPH_X + GRAPH_WIDTH * t / (STEP * ITERATION);
}

float charge_to_graphY(float q)
{
	return GRAPH_Y + GRAPH_HEIGHT * (1.f - q / Q_NOT);
}

void discharge(float tn, const float *yn, float *out);

void draw_measurements_table_outline()
{
	Rectangle table = { TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT };
	DrawRectangleLinesEx(table, 2.f, BLACK);
	Vector2 dividing_line_start = { TABLE_X + TABLE_WIDTH / 2.f, TABLE_Y };
	Vector2 dividing_line_end = { TABLE_X + TABLE_WIDTH / 2.f,
				      TABLE_Y + TABLE_HEIGHT };
	DrawLineEx(dividing_line_start, dividing_line_end, 2.f, BLACK);
	Vector2 heading_line_start = { TABLE_X, TABLE_Y + (float)CELL_WIDTH };
	Vector2 heading_line_end = { TABLE_X + TABLE_WIDTH,
				     TABLE_Y + (float)CELL_WIDTH };

	DrawLineEx(heading_line_start, heading_line_end, 2.f, BLACK);

	for (int i = 1; i < TABLE_NUM_ROWS; i++) {
		int y_pos = TABLE_Y + (i + 1) * (float)CELL_WIDTH;
		DrawLine(TABLE_X, y_pos, TABLE_X + TABLE_WIDTH, y_pos, BLACK);
	}

	DrawText(" Time (tn)", TABLE_X + TABLE_TEXT_OFFSET_X, TABLE_Y + 5, 20,
		 BLACK);
	DrawText(" Charge (qn)",
		 TABLE_X + TABLE_WIDTH / 2.f + TABLE_TEXT_OFFSET_X, TABLE_Y + 5,
		 20, BLACK);
}

int main()
{
	float *tns = (float *)malloc(sizeof(float) * (ITERATION + 1));
	float *qns = (float *)malloc(sizeof(float) * (ITERATION + 1));
	tns[0] = 0.f;
	qns[0] = Q_NOT;
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Discharging a Capacitor");
	SetTargetFPS(10);
	float tn = 0.0;
	float qn = Q_NOT;
	char text_buffer[11];
	int iter = 1;
	Rk2 *rk2 = (Rk2 *)malloc(sizeof(Rk2));
	rk2->dimension = 1;
	rk2_init_ralston(rk2, discharge, STEP);

	while (!WindowShouldClose()) {
		if (iter <= ITERATION) {
			rk2_apply_update(rk2, tn, &qn);
			tn += STEP;
			tns[iter] = tn;
			qns[iter] = qn;
			iter++;
		}
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangle(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT,
			      SKYBLUE);

		draw_measurements_table_outline();
		for (int i = 1; i < iter; i++) {
			float x1 = time_to_graphX(tns[i - 1]);
			float x2 = time_to_graphX(tns[i]);
			float y1 = charge_to_graphY(qns[i - 1]);
			float y2 = charge_to_graphY(qns[i]);
			Vector2 v1 = { x1, y1 };
			Vector2 v2 = { x2, y2 };
			DrawLineEx(v1, v2, 4.f, BLACK);

			// DrawCircle(time_to_graphX(tns[i]),
			// 	   charge_to_graphY(qns[i]), 2.f, RED);
		}

		for (int i = 1; i < TABLE_NUM_ROWS && i < iter; i++) {
			sprintf(text_buffer, "%10.3f", tns[i - 1]);
			DrawText(text_buffer, TABLE_X + TABLE_TEXT_OFFSET_X,
				 (i + 1) * CELL_WIDTH + 5, 20, BLACK);
			sprintf(text_buffer, "%10.3f", qns[i - 1]);
			DrawText(text_buffer,
				 TABLE_X + TABLE_WIDTH / 2.f +
					 TABLE_TEXT_OFFSET_X,
				 (i + 1) * CELL_WIDTH + 5, 20, BLACK);
		}

		EndDrawing();
	}
	CloseWindow();
	return 0;
}

void discharge(float tn, const float *yn, float *out)
{
	(void)tn;
	out[0] = -yn[0] / (R * C);
}
