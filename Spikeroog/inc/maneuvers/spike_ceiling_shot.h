#pragma once
#include "mechanics/dodge.h"
#include "mechanics/drive.h"
#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"


class SpikeCeilingShot : public Maneuver
{
    int phase = 0;
    
    Travel travel;
    Dodge dodge;
    Drive drive;


public:
    SpikeCeilingShot(GameInfo & info);

    void step();
    //void render(Renderer & renderer);
};