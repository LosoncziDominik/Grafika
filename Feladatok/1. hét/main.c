#include "brick.h"

#include <stdio.h>

int main()
{
	Brick brick;
	double area;
	
	set_brick_data(&brick, 5, 10);
	area = calc_brick_area(&brick);
	
	printf("Brick area: %lf\n", area);
	
	return 0;
}
