#pragma once
#include "rlbot/bot.h"
#include "simulation/game.h"
#include "simulation/car.h"
#include "rlbot/renderer.h"
#include "utils/game_info.h"
#include "maneuvers/maneuver.h"
#include <memory>


class Spikeroog : public Bot
{
    GameInfo info;
    std::shared_ptr<Maneuver> maneuver;
    Renderer renderer;

    float nextStateSetTime = 5.0f;
    bool hasCancelledManeuverDueToKickoff = false;

public:
    Spikeroog(int index, int team, std::string name, Game & game);

    Input GetOutput(Game game);

    std::shared_ptr<Maneuver> chooseManeuver();
};