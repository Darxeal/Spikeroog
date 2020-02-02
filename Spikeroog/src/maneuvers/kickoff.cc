#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "mechanics/dodge.h"
#include "mechanics/drive.h"
#include <memory>
#include "maneuvers/kickoff.h"

Kickoff::Kickoff(GameInfo & info) : Maneuver(info), travel(info.myCar), dodge(info.myCar)
{
    travel.target = normalize(info.myGoal) * 150.0f;
    travel.noActionDistance = -500.0f;
}

void Kickoff::step()
{
    if (dodging)
    {
        dodge.direction = direction(info.myCar.position, info.ball.position);
        dodge.step(info.dt);
        controls = dodge.controls;
        finished = dodge.finished;
    }
    else
    {
        travel.step(info.dt);
        controls = travel.controls;

        // check if an opponent is going for kickoff
        bool isOpponentFaking = true;
        for (auto opponent : info.opponents)
        {
            if (distance(opponent->position, info.ball.position) < 1500)
            {
                isOpponentFaking = false;
                break;
            }
        }

        if (ground_distance(info.myCar.position, vec3(0)) < 900 and not isOpponentFaking)
            dodging = true;
    }
}

void Kickoff::render(Renderer & renderer)
{
    travel.render(renderer);
}