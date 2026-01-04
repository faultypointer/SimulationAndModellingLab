#include <stdio.h>
#include "../../external/raylib-5.5/src/raylib.h"
#include "../include/sam.h"
#include <stdlib.h>
#include <math.h>

#define DAMPING_FACTOR \
	0.3f // Try: 0.0f (undamped), 0.3f (underdamped), 1.0f (critically damped), 2.0f (overdamped)
#define SPRING_CONSTANT 5.0f
#define LOAD_MASS 1.0f
#define X_NOT 1.0f
#define V_NOT 0.0f
#define ITERATION 100
#define STEP 0.1f

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GRAPH_X 30
#define GRAPH_Y 120
#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 570
#define TABLE_X 660
#define TABLE_Y 120
#define TABLE_WIDTH 580
#define TABLE_HEIGHT 570
#define TABLE_NUM_ROWS 17
#define TABLE_TEXT_OFFSET_X 30
#define CELL_WIDTH TABLE_HEIGHT / (TABLE_NUM_ROWS + 1)

void update(float tn, const float *yn, float *out)
{
	(void)tn;
	float x = yn[0];
	float v = yn[1];
	out[0] = v;
	out[1] = -(DAMPING_FACTOR / LOAD_MASS) * v -
		 (SPRING_CONSTANT / LOAD_MASS) * x;
}

void draw_measurements_table_outline()
{
	Rectangle table = { TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT };
	DrawRectangleLinesEx(table, 3.f, DARKGRAY);

	// Header background
	DrawRectangle(TABLE_X, TABLE_Y, TABLE_WIDTH, CELL_WIDTH, LIGHTGRAY);

	Vector2 dividing_line_start = { TABLE_X + TABLE_WIDTH / 2.f, TABLE_Y };
	Vector2 dividing_line_end = { TABLE_X + TABLE_WIDTH / 2.f,
				      TABLE_Y + TABLE_HEIGHT };
	DrawLineEx(dividing_line_start, dividing_line_end, 3.f, DARKGRAY);

	Vector2 heading_line_start = { TABLE_X, TABLE_Y + (float)CELL_WIDTH };
	Vector2 heading_line_end = { TABLE_X + TABLE_WIDTH,
				     TABLE_Y + (float)CELL_WIDTH };
	DrawLineEx(heading_line_start, heading_line_end, 3.f, DARKGRAY);

	for (int i = 1; i < TABLE_NUM_ROWS; i++) {
		int y_pos = TABLE_Y + (i + 1) * (float)CELL_WIDTH;
		DrawLine(TABLE_X, y_pos, TABLE_X + TABLE_WIDTH, y_pos, GRAY);
	}

	DrawText("Time (tn)", TABLE_X + TABLE_TEXT_OFFSET_X + 50, TABLE_Y + 10,
		 22, BLACK);
	DrawText("Position (xn)",
		 TABLE_X + TABLE_WIDTH / 2.f + TABLE_TEXT_OFFSET_X + 30,
		 TABLE_Y + 10, 22, BLACK);
}

float time_to_graphX(float t, float max_time)
{
	return GRAPH_X + GRAPH_WIDTH * t / max_time;
}

float position_to_graphY(float x, float min_x, float max_x)
{
	float range = max_x - min_x;
	if (range < 0.001f)
		range = 2.0f;
	return GRAPH_Y + GRAPH_HEIGHT - (GRAPH_HEIGHT * (x - min_x) / range);
}

void find_min_max(float states[][2], int count, float *min_x, float *max_x)
{
	*min_x = states[0][0];
	*max_x = states[0][0];

	for (int i = 0; i < count; i++) {
		if (states[i][0] < *min_x)
			*min_x = states[i][0];
		if (states[i][0] > *max_x)
			*max_x = states[i][0];
	}

	// Add 10% padding
	float padding = (*max_x - *min_x) * 0.1f;
	if (padding < 0.1f)
		padding = 0.1f;
	*min_x -= padding;
	*max_x += padding;
}

const char *get_damping_type()
{
	float critical_damping = 2.0f * sqrtf(SPRING_CONSTANT * LOAD_MASS);
	float damping_ratio = DAMPING_FACTOR / critical_damping;

	if (damping_ratio < 0.01f)
		return "Undamped";
	else if (damping_ratio < 1.0f)
		return "Underdamped";
	else if (damping_ratio < 1.1f)
		return "Critically Damped";
	else
		return "Overdamped";
}

int main()
{
	Rk2 solver;
	rk2_init_midpoint(&solver, update, STEP);
	solver.dimension = 2;

	char text_buffer[30];
	float *tns = (float *)malloc((sizeof(float) * ITERATION));
	float states[ITERATION][2];

	tns[0] = 0.f;
	states[0][0] = X_NOT;
	states[0][1] = V_NOT;

	float current_state[2] = { states[0][0], states[0][1] };
	int iter = 1;

	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
		   "Spring-Mass System Simulation");
	SetTargetFPS(15);

	while (!WindowShouldClose()) {
		if (iter < ITERATION) {
			rk2_apply_update(&solver, tns[iter - 1], current_state);
			tns[iter] = tns[iter - 1] + STEP;
			states[iter][0] = current_state[0];
			states[iter][1] = current_state[1];
			iter++;
		}

		float min_x, max_x;
		find_min_max(states, iter, &min_x, &max_x);
		float max_time = tns[iter - 1];

		BeginDrawing();
		ClearBackground(RAYWHITE);

		Color header_bg = { 240, 245, 255, 255 };
		DrawRectangle(0, 0, WINDOW_WIDTH, 110, header_bg);
		DrawLine(0, 110, WINDOW_WIDTH, 110, DARKGRAY);

		DrawText("Spring-Mass System Simulation", 350, 10, 32,
			 DARKBLUE);

		sprintf(text_buffer, "Damping: %.2f", DAMPING_FACTOR);
		DrawText(text_buffer, 30, 50, 25, DARKGRAY);
		sprintf(text_buffer, "Spring K: %.2f", SPRING_CONSTANT);
		DrawText(text_buffer, 300, 50, 25, DARKGRAY);
		sprintf(text_buffer, "Mass: %.2f", LOAD_MASS);
		DrawText(text_buffer, 580, 50, 25, DARKGRAY);
		sprintf(text_buffer, "Type: %s", get_damping_type());
		DrawText(text_buffer, 830, 50, 25, DARKGREEN);

		sprintf(text_buffer, "Initial Position: %.2f m", X_NOT);
		DrawText(text_buffer, 30, 80, 18, GRAY);
		sprintf(text_buffer, "Initial Velocity: %.2f m/s", V_NOT);
		DrawText(text_buffer, 250, 80, 18, GRAY);
		sprintf(text_buffer, "Time Step: %.2f s", STEP);
		DrawText(text_buffer, 490, 80, 18, GRAY);

		Color graph_bg = { 230, 240, 255, 255 };
		DrawRectangle(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT,
			      graph_bg);
		DrawRectangleLinesEx((Rectangle){ GRAPH_X, GRAPH_Y, GRAPH_WIDTH,
						  GRAPH_HEIGHT },
				     3.f, DARKGRAY);

		Color grid_color = { 200, 210, 230, 255 };
		for (int i = 1; i < 5; i++) {
			int y = GRAPH_Y + i * GRAPH_HEIGHT / 5;
			DrawLine(GRAPH_X, y, GRAPH_X + GRAPH_WIDTH, y,
				 grid_color);
		}
		for (int i = 1; i < 5; i++) {
			int x = GRAPH_X + i * GRAPH_WIDTH / 5;
			DrawLine(x, GRAPH_Y, x, GRAPH_Y + GRAPH_HEIGHT,
				 grid_color);
		}

		Color zero_line = { 100, 100, 100, 150 };
		float zero_y = position_to_graphY(0.0f, min_x, max_x);
		DrawLine(GRAPH_X, zero_y, GRAPH_X + GRAPH_WIDTH, zero_y,
			 zero_line);

		for (int i = 1; i < iter; i++) {
			float x1 = time_to_graphX(tns[i - 1], max_time);
			float x2 = time_to_graphX(tns[i], max_time);
			float y1 = position_to_graphY(states[i - 1][0], min_x,
						      max_x);
			float y2 =
				position_to_graphY(states[i][0], min_x, max_x);

			Vector2 v1 = { x1, y1 };
			Vector2 v2 = { x2, y2 };
			DrawLineEx(v1, v2, 3.f, BLUE);
		}

		if (iter > 0) {
			float curr_x = time_to_graphX(tns[iter - 1], max_time);
			float curr_y = position_to_graphY(states[iter - 1][0],
							  min_x, max_x);
			DrawCircle(curr_x, curr_y, 6.f, RED);
		}

		DrawText("Position vs Time", GRAPH_X + 200, GRAPH_Y - 30, 22,
			 DARKBLUE);
		sprintf(text_buffer, "%.2f", max_x);
		DrawText(text_buffer, GRAPH_X - 50, GRAPH_Y + 5, 16, DARKGRAY);
		sprintf(text_buffer, "%.2f", min_x);
		DrawText(text_buffer, GRAPH_X - 50, GRAPH_Y + GRAPH_HEIGHT - 20,
			 16, DARKGRAY);
		sprintf(text_buffer, "%.1f", max_time);
		DrawText(text_buffer, GRAPH_X + GRAPH_WIDTH - 30,
			 GRAPH_Y + GRAPH_HEIGHT + 10, 16, DARKGRAY);
		DrawText("Time (s)", GRAPH_X + GRAPH_WIDTH / 2 - 30,
			 GRAPH_Y + GRAPH_HEIGHT + 10, 16, DARKGRAY);
		DrawText("x(m)", GRAPH_X - 50, GRAPH_Y + GRAPH_HEIGHT / 2, 16,
			 DARKGRAY);

		draw_measurements_table_outline();

		int table_start =
			(iter > TABLE_NUM_ROWS) ? iter - TABLE_NUM_ROWS : 0;
		for (int i = 0; i < TABLE_NUM_ROWS && (table_start + i) < iter;
		     i++) {
			int idx = table_start + i;
			sprintf(text_buffer, "%.3f", tns[idx]);
			DrawText(text_buffer,
				 TABLE_X + TABLE_TEXT_OFFSET_X + 80,
				 TABLE_Y + (i + 1) * CELL_WIDTH + 10, 20,
				 BLACK);
			sprintf(text_buffer, "%.3f", states[idx][0]);
			DrawText(text_buffer,
				 TABLE_X + TABLE_WIDTH / 2.f +
					 TABLE_TEXT_OFFSET_X + 80,
				 TABLE_Y + (i + 1) * CELL_WIDTH + 10, 20,
				 BLACK);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
