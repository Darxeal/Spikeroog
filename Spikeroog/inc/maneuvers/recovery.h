#pragma once
#include "mechanics/reorient.h"
#include "simulation/car.h"
#include "simulation/input.h"
#include "maneuvers/maneuver.h"

class Recovery : public Maneuver
{
    Reorient reorient;

public:
    Recovery(GameInfo & info);

    void step();
};
    
