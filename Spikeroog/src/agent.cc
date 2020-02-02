#include "rlbot/bot.h"
#include "mechanics/drive.h"
#include "simulation/game.h"
#include "simulation/car.h"
#include "simulation/field.h"
#include "rlbot/renderer.h"
#include "agent.h"
#include <memory>
#include "utils/misc.h"
#include "maneuvers/pickup_ball.h"
#include "maneuvers/challenge.h"
#include "maneuvers/spike_shot.h"
#include "maneuvers/spike_ceiling_shot.h"
#include "mechanics/travel.h"
#include "maneuvers/kickoff.h"
#include "maneuvers/defense.h"
#include "maneuvers/recovery.h"
#include "maneuvers/collect_boost.h"
#include "maneuvers/walldash_spike_carry.h"
#include <time.h>

Spikeroog::Spikeroog(int index, int team, std::string name, Game & game)
: Bot(index, team, name), renderer(index), info(game, index, team)
{
    Field::initialize_soccar();
    maneuver = nullptr;
    srand(time(NULL)); // random seed for state setting
}

Input Spikeroog::GetOutput(Game game)
{
    info.update();
    
    // state setting for testing purposes
    #if 0
    if (game.time > nextStateSetTime)
    {
        Game desiredGame(game);

        // desiredGame.ball.position = vec3{3500, -3000, 90};
        // desiredGame.ball.velocity = vec3{0,0,0};

        Car & myDesiredCar = desiredGame.cars[index];
        myDesiredCar.position = info.myGoal * 0.3f + vec3{(float)(rand() % 3000),0,0};
        // myDesiredCar.velocity = vec3{0,1000,0};
        // myDesiredCar.o = look_at({0,1,0}, {0,0,1});

        desiredGame.SetState();
        nextStateSetTime += 9.0f;
        maneuver = nullptr;
    }

    if (game.time < nextStateSetTime - 8.8f)
        return Input();
    #endif

    // we need to cancel current maneuver when kickoff happens, if we didn't do it already
    if (game.kickoff_pause and not hasCancelledManeuverDueToKickoff)
    {
        maneuver = nullptr;
        hasCancelledManeuverDueToKickoff = true;
    }

    // cancel finished maneuver
    if (maneuver and maneuver->finished)
    {
        maneuver = nullptr;
        hasCancelledManeuverDueToKickoff = false;
    }

    // choose a new maneuver
    if (not maneuver)
        maneuver = chooseManeuver();

    // execute maneuver
    maneuver->step();

    // rendering
    renderer.Start();
    maneuver->render(renderer);
    renderer.DrawString3D({255,255,255,255}, typeid(*maneuver).name(), info.myCar.position, 1, 1);
    renderer.Finish();

    // finally, return the controls
    return maneuver->controls;
}

std::shared_ptr<Maneuver> Spikeroog::chooseManeuver()
{
    // I'm airborne
    if (not info.myCar.on_ground and info.myCar.position[2] > 50)
        return std::make_shared<Recovery>(info);

    // I have the ball
    if (isBallAttached(info.myCar, info))
    {
        if (abs(info.ball.position[0]) > 2000 and ground_distance(info.ball.position, info.theirGoal) > 3500)
        {
            if (abs(info.ball.position[1]) < 3000)
                return std::make_shared<SpikeCeilingShot>(info);
            else
                return std::make_shared<WallDashSpikeCarry>(info);
        }
        else
            return std::make_shared<SpikeShot>(info);
    }

    // an opponent has the ball
    for (auto opponent : info.opponents)
    {
        if (isBallAttached(*opponent, info))
            return std::make_shared<Challenge>(info, *opponent);
    }
    
    // if ball is far from our goal
    if (distance(info.ball.position + info.ball.velocity * 2.0f, info.myGoal) > 3000)
    {
        // dont get in the way of our teammate
        for (auto teammate : info.teammates)
        {
            if (distance(teammate->position, info.ball.position) < distance(info.myCar.position, info.ball.position) - 50.0f)
            {
                if (isBallAttached(*teammate, info)
                and info.myCar.boost < 30
                and distance(info.ball.position, info.myGoal) > 7000)
                    return std::make_shared<CollectBoost>(info);
                else
                    return std::make_shared<Defense>(info);
            }
        }
    }

    // kickoff
    if(norm(xy(info.ball.position)) < 0.01f)
        return std::make_shared<Kickoff>(info);

    // noone has the ball
    return std::make_shared<PickupBall>(info);
}