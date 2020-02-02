#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "mechanics/drive.h"
#include <memory>
#include "maneuvers/walldash_spike_carry.h"


WallDashSpikeCarry::WallDashSpikeCarry(GameInfo & info)
 : Maneuver(info), travel(info.myCar), walldash(info.myCar)
{
}

void WallDashSpikeCarry::step()
{
    Car & car = info.myCar;
    vec3 target = info.theirGoal;
    target[2] = 500;

    if (dashing)
    {
        if (distance(car.position, target) < 3000)
            walldash.target = target;
        else
        {
            walldash.target = car.position + ground_direction(car.position, target) * 100;
            walldash.target[2] = clip(car.position[2] + 20.0f, startingHeight, 1000.0f);
        }

        walldash.step(info.dt);
        controls = walldash.controls;
    }
    else
    {
        if (car.position[2] < 110)
            travel.target = vec3{sgn(car.position[0]) * 6000, car.position[1], 5000} + direction(car.position, target) * 2000;
        else
            travel.target = car.position + ground_direction(car.position, target) * 100;

        travel.step(info.dt);
        controls = travel.controls;

        dashing = car.position[2] > 150;
        startingHeight = car.position[2];
    }

    finished = not isBallAttached(car, info) or (dashing and car.position[2] < 120);
}