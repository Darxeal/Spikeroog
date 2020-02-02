#pragma once
#include "simulation/game.h"
#include "simulation/pad.h"


class GameInfo
{
public:
    Game & game;

    Car & myCar;
    std::vector<Car *> teammates;
    std::vector<Car *> opponents;

    Ball & ball;
    float & dt;
    float & time;

    std::vector<Pad *> largeBoostPads;
    std::vector<Pad *> smallBoostPads;

    vec3 myGoal;
    vec3 theirGoal;

    GameInfo(Game & game, int index, int team) : game(game), myCar(game.cars[index]), ball(game.ball), dt(game.time_delta), time(game.time)
    {
        vec3 blueGoal{0.0f, -5120.0f, 0.0f};
        vec3 orangeGoal{0.0f, 5120.0f, 0.0f};
        myGoal = team == 0 ? blueGoal : orangeGoal;
        theirGoal = team == 1 ? blueGoal : orangeGoal;
    };

    void update()
    {
        teammates.clear();
        opponents.clear();

        for (size_t i = 0; i < game.num_cars; i++)
        {
            if(game.cars[i].team == myCar.team)
            {
                if (game.cars[i].id != myCar.id)
                    teammates.push_back(&game.cars[i]);
            }
            else
            {
                opponents.push_back(&game.cars[i]);
            }
        }
        
        smallBoostPads.clear();
        largeBoostPads.clear();

        for (size_t i = 0; i < game.pads.size(); i++)
        {
            game.pads[i].is_full_boost ? largeBoostPads.push_back(&game.pads[i]) : smallBoostPads.push_back(&game.pads[i]);
        }
        
    }


};