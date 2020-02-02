#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "simulation/ball.h"
#include "simulation/field.h"
#include "maneuvers/pickup_ball.h"

PickupBall::PickupBall(GameInfo & info) : Maneuver(info), travel(info.myCar), drive(info.myCar) {};

void PickupBall::step()
{
    Ball ballCopy = info.ball;
    const float distFromBall = 150.0f;

    float idealSpeed;
    vec3 target;

    while(true)
    {
        ballCopy.step(1.0f / 60.0f);
        target = ballCopy.position + ground_direction(ballCopy.position, info.myCar.position) * distFromBall;
        float timeLeft = ballCopy.time - info.time;

        idealSpeed = ground_distance(target, info.myCar.position)/timeLeft;
        bool canReach = idealSpeed < travel.estimateMaxSpeed(timeLeft);
        bool wallNearby = norm(Field::collide({ballCopy.position, 150.0f}).direction) > 0.0;

        if ((canReach && wallNearby) || timeLeft > 10.0f)
        {
            intercept = ballCopy;
            break;
        }
    }
    
    if (idealSpeed > 1000)
    {
        travel.target = target;
        travel.step(info.dt);
        controls = travel.controls;
    }
    else
    {
        drive.target = target;
        drive.speed = idealSpeed;
        drive.step(info.dt);
        controls = drive.controls;
    }

    // cancel when we or any other car picked up the ball
    finished = false;
    for(auto player : info.game.cars)
    {
        if (isBallAttached(player, info))
            finished = true;
    }
}

void PickupBall::render(Renderer & renderer)
{
    renderer.DrawSphere({255,255,0,255}, intercept.hitbox());
    travel.render(renderer);
}