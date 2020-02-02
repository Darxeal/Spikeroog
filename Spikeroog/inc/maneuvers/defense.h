#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "mechanics/drive.h"
#include "utils/vector_math.h"
#include "simulation/ball.h"

class Defense : public Maneuver
{
    Travel travel;
    Drive drive;

    bool parking = false;

public:
    Defense(GameInfo & info);

    void step();
    void render(Renderer & renderer);
};