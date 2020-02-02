#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "mechanics/dodge.h"
#include "mechanics/wavedash.h"
#include "mechanics/drive.h"
#include <memory>
#include "mechanics/travel.h"
#include "simulation/field.h"

Travel::Travel(Car & car) : car(car), drive(car)
{
    drive.speed = 2300;
}

float Travel::estimateMaxSpeed(float timeLeft)
{
    int possibleDodges = timeLeft / (dodgeDuration + groundRestTime);
    float baseSpeed = std::max(norm(car.velocity), 1300.0f);
    float boostSpeed = 1000.0f * std::min(timeLeft, car.boost / 33.0f);
    return clip(baseSpeed + boostSpeed + possibleDodges * 500.f, 0.0f, 2300.0f);
}

void Travel::step(float dt)
{
    drive.target = target;   

    float carVelocity = norm(car.velocity);
    float timeLeft = (distance(car.position, target) - noActionDistance) / fmax(carVelocity + 500, 1400);
    float angleToTarget = angle_to(car, target);

    // driving
    if (not dodging and not wavedashing and car.on_ground)
    {
        timeSpentOnGround += dt;

        // start dodging when appropriate
        if (timeSpentOnGround > groundRestTime
        and car.position[2] < 200
        and angleToTarget < 0.2
        and carVelocity > 1200
        and carVelocity < 2100)
        {
            if (allowDodges and timeLeft > dodgeDuration)
            {
                dodge = std::make_unique<Dodge>(car);
                dodging = true;
            }
            else if (allowWavedashes and timeLeft > wavedashDuration)
            {
                wavedash = std::make_unique<Wavedash>(car);
                wavedashing = true;
            }
            
        }
    }

    if (dodging) // dodge
    {
        dodge->jump_duration = 0.1f;
        dodge->direction = direction(car.position, target);

        dodge->step(dt);
        controls = dodge->controls;
    }
    else if (wavedashing) // wavedash
    {
        wavedash->direction = direction(car.position, target);

        wavedash->step(dt);
        controls = wavedash->controls;
    }
    else // drive
    {
        drive.step(dt);
        controls = drive.controls;
        controls.jump = false;

        // dont boost while turning
        if (angleToTarget > 0.3)
            controls.boost = false;

        // powerslide
        controls.handbrake = angleToTarget > 1.5f && car.position[2] < 300;
    }

    // dont boost in the air
    if (not car.on_ground)
        controls.boost = false;

    // dodge finished
    if (dodging and dodge->finished)
    {
        dodging = false;
        timeSpentOnGround = 0;
    }
    // wavedash finished
    if (wavedashing and wavedash->finished)
    {
        wavedashing = false;
        timeSpentOnGround = 0;
    }
}

void Travel::render(Renderer & renderer)
{
    renderer.DrawLine3D({255,255,0,255}, target - vec3{100, 0, 0}, target + vec3{100, 0, 0});
    renderer.DrawLine3D({255,255,0,255}, target - vec3{0, 100, 0}, target + vec3{0, 100, 0});
    renderer.DrawLine3D({255,255,0,255}, target - vec3{0, 0, 100}, target + vec3{0, 0, 100});
}