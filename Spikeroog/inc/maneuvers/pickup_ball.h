#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "mechanics/drive.h"
#include "utils/vector_math.h"
#include "simulation/ball.h"

class PickupBall : public Maneuver
{
    Ball intercept;
    Travel travel;
    Drive drive;

public:
    PickupBall(GameInfo & info);

    void step();
    void render(Renderer & renderer);
};