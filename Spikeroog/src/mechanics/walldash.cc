#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "mechanics/drive.h"
#include <memory>
#include "mechanics/walldash.h"


WallDash::WallDash(Car & car) : car(car), drive(car)
{
    drive.speed = 2300;
}

bool WallDash::isViable()
{
    return car.position[2] > 150 and abs(car.forward()[2]) < 0.5 and norm(car.velocity) > 500;
}

void WallDash::step(float dt)
{
    timer += dt;
    controls.jump = false;
    controls.pitch = 0;
    controls.roll = 0;

    // alternate jump
    if (timer > 0.06666f)
    {
        controls.jump = true;
        timer = 0;
        controls.pitch = -1;
        controls.roll = sgn(car.left()[2]); // roll towards wall
    }

    drive.target = target;
    drive.step(dt);
    controls.steer = drive.controls.steer;
    controls.throttle = 1;
}