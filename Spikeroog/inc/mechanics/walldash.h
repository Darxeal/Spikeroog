#pragma once
#include "mechanics/drive.h"
#include "simulation/car.h"
#include "simulation/input.h"


class WallDash
{
    Drive drive;
    float timer = 0.0f;

public:
    Car & car;
    Input controls;

    vec3 target;

    WallDash(Car & info);

    bool isViable();

    void step(float dt);
};
    
