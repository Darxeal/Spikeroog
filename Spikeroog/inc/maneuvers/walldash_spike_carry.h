#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "mechanics/drive.h"
#include "mechanics/walldash.h"


class WallDashSpikeCarry : public Maneuver
{
    Travel travel;
    WallDash walldash;
    bool dashing = false;
    float startingHeight;

public:

    WallDashSpikeCarry(GameInfo & info);

    void step();
};
    
