#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "simulation/ball.h"
#include "maneuvers/challenge.h"

Challenge::Challenge(GameInfo & info, Car & targetCar)
: Maneuver(info), travel(info.myCar), targetCar(targetCar)
{
    travel.noActionDistance = 500.0f;
};

void Challenge::step()
{
    vec3 target;
    float timeLeft = 0;

    while(true)
    {
        timeLeft += 1.0f/60.f;
        vec3 expectedTargetCarDirection =
            normalize(direction(targetCar.position, info.myGoal) * 2 + normalize(targetCar.velocity));

        target = targetCar.position + expectedTargetCarDirection * norm(targetCar.velocity) * timeLeft;
        target += info.ball.position - targetCar.position;

        float distanceToTarget = distance(target, info.myCar.position);

        if (distanceToTarget/timeLeft < travel.estimateMaxSpeed(timeLeft) or timeLeft > 10.0f)
        {
            target = target;
            break;
        }
    }

    travel.target = target;
    travel.step(info.dt);
    controls = travel.controls;
    
    if (distance(info.myCar.position, target) < 300.0f and targetCar.position[2] > info.myCar.position[2] + 30.0f)
        controls.jump = true;

    finished = not isBallAttached(targetCar, info);
}

void Challenge::render(Renderer & renderer)
{
    travel.render(renderer);
}