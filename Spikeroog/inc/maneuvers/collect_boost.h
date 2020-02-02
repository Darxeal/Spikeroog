#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "mechanics/drive.h"
#include "utils/vector_math.h"
#include "simulation/ball.h"
#include "simulation/pad.h"

class CollectBoost : public Maneuver
{
    Travel travel;

public:
    CollectBoost(GameInfo & info);

    void step();
    void render(Renderer & renderer);
};