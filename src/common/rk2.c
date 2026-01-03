#include "../include/sam.h"

Rk2 *rk2_init_midpoint(Rk2 *rk2, rk2_update_func update, float step)
{
	rk2->update = update;
	rk2->alpha = 0.5f;
	rk2->beta = 0.5;
	rk2->a = 0.f;
	rk2->b = 1.f;
	rk2->step = step;
	return rk2;
}

Rk2 *rk2_init_ralston(Rk2 *rk2, rk2_update_func update, float step)
{
	rk2->update = update;
	rk2->alpha = 0.75f;
	rk2->beta = 0.75;
	rk2->a = 0.333333f;
	rk2->b = 0.67f;
	rk2->step = step;
	return rk2;
}

void rk2_apply_update(Rk2 *rk2, float tn, float *yn)
{
	int n = rk2->dimension;
	float k1[n], k2[n], y_tmp[n];
	rk2->update(tn, yn, k1);
	for (int i = 0; i < n; i++) {
		y_tmp[i] = yn[i] + (rk2->beta * rk2->step * k1[i]);
	}
	rk2->update(tn + rk2->alpha * rk2->step, y_tmp, k2);
	for (int i = 0; i < n; i++) {
		yn[i] = yn[i] + rk2->step * (rk2->a * k1[i] + rk2->b * k2[i]);
	}
}
