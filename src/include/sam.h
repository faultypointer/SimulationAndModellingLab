#ifndef SAM_H
#define SAM_H

#include <stdlib.h>

typedef float (*rk2_update_func)(float, float);

typedef struct {
	float alpha;
	float beta;
	float a;
	float b;
	float step;

	rk2_update_func update;
} Rk2;

Rk2 *rk2_init_midpoint(Rk2 *rk2, rk2_update_func update, float step);
Rk2 *rk2_init_ralston(Rk2 *rk2, rk2_update_func update, float step);
float rk2_apply_update(Rk2 *rk2, float tn, float yn);

#endif
