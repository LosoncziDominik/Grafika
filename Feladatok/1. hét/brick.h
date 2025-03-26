#ifndef BRICK_H
#define BRICK_H

    typedef struct Brick{
        double a;
        double b;
    }Brick;

void set_brick_data(Brick* brick, double a, double b);

double calc_brick_area(const Brick* brick);

#endif  