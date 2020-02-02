#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "simulation/ball.h"
#include "simulation/field.h"
#include "maneuvers/defense.h"

Defense::Defense(GameInfo & info) : Maneuver(info), travel(info.myCar), drive(info.myCar)
{
    travel.target = info.myGoal;
    travel.allowWavedashes = false;
    travel.noActionDistance = 500.0f;
    drive.target = info.theirGoal;
};

void Defense::step()
{
    if (ground_distance(info.myCar.position, info.myGoal) < 800)
    {
        controls = Input();
        controls.handbrake = true;
        drive.step(info.dt);
        controls.steer = drive.controls.steer;
        finished = true;
    }
    else
    {
        travel.step(info.dt);
        controls = travel.controls;
        controls.boost = false;
    }

    if (isBallAttached(info.myCar, info))
        finished = true;
}

void Defense::render(Renderer & renderer)
{
    travel.render(renderer);
}