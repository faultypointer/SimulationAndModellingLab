#ifndef SAM_H
#define SAM_H

#include <stdlib.h>

typedef void (*rk2_update_func)(float tn, const float *yn, float *out);

typedef struct {
	float alpha;
	float beta;
	float a;
	float b;
	float step;

	int dimension;
	rk2_update_func update;
} Rk2;

void rk2_apply_update(Rk2 *rk2, float tn, float *yn);

Rk2 *rk2_init_midpoint(Rk2 *rk2, rk2_update_func update, float step);
Rk2 *rk2_init_ralston(Rk2 *rk2, rk2_update_func update, float step);

#endif
