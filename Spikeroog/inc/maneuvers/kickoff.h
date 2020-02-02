#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/dodge.h"
#include "mechanics/travel.h"

class Kickoff : public Maneuver
{
    Travel travel;
    Dodge dodge;

    bool dodging = false;

public:
    Kickoff(GameInfo & info);

    void step();
    void render(Renderer & renderer);
};