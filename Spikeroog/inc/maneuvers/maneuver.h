#pragma once
#include "simulation/input.h"
#include "rlbot/renderer.h"
#include "utils/game_info.h"

class Maneuver
{
public:
    GameInfo & info;
    Input controls;
    bool finished = false;

    Maneuver(GameInfo & info) : info(info) {};

    virtual void step() = 0;

    virtual void render(Renderer & renderer) {};
};
