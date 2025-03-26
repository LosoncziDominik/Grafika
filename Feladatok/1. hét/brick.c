#include "Brick.h"

#include <math.h>

void set_brick_data(Brick* brick, double a, double b)
{
	brick->a = a;
	brick->b = b;
}


double calc_brick_area(const Brick* brick)
{
	double area = brick->a * brick->b;
	return area;
}
