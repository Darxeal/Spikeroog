#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "mechanics/dodge.h"
#include "mechanics/drive.h"
#include <memory>
#include "maneuvers/spike_shot.h"

SpikeShot::SpikeShot(GameInfo & info) : Maneuver(info), travel(info.myCar), dodge(info.myCar)
{
    travel.target = info.theirGoal;
    travel.allowDodges = false;

    dodge.jump_duration = 0.2f;
    dodge.delay = 0.6f;
}

void SpikeShot::step()
{
    timer += info.dt;

    if (dodging)
    {
        dodge.step(info.dt);
        controls = dodge.controls;
        finished = dodge.finished;
        if (dodge.timer > dodge.delay + 0.13f)
            controls.useItem = true;
    }
    else
    {
        // travel towards goal
        travel.step(info.dt);
        controls = travel.controls;
        finished = not isBallAttached(info.myCar, info);

        // check if an opponent is near
        bool isOpponentNear = false;
        for (auto opponent : info.opponents)
        {
            if (ground_distance(info.myCar.position, opponent->position + opponent->velocity) < 2500.0f)
            {
                isOpponentNear = true;
                break;
            }
        }

        // shoot when facing goal and opponent is near or we have enough speed
        bool shouldFlick = isOpponentNear or norm(info.myCar.velocity) > 1800.0f;

        if (timer > 0.2f and info.myCar.on_ground and shouldFlick
        and angle_between(xy(info.myCar.forward()), ground_direction(info.myCar.position, info.theirGoal)) < 0.2f)
        {
            dodging = true;

            // set preorientation to put the ball higher
            vec3 preorientationDirection = direction(info.myCar.position, info.theirGoal);
            preorientationDirection[2] = 5.0f;
            dodge.preorientation = look_at(normalize(preorientationDirection));

            dodge.direction = direction(info.myCar.position, info.theirGoal);
        }
    }
    
}

void SpikeShot::render(Renderer & renderer)
{
    travel.render(renderer);
}