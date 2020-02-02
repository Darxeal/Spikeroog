#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/dodge.h"
#include "mechanics/travel.h"

class SpikeShot : public Maneuver
{
    Travel travel;
    Dodge dodge;

    bool dodging = false;
    float timer = 0.0f;

public:
    SpikeShot(GameInfo & info);

    void step();
    void render(Renderer & renderer);
};