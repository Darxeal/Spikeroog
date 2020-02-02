#pragma once
#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "utils/vector_math.h"
#include "simulation/ball.h"

class Challenge : public Maneuver
{
    Car & targetCar;
    Travel travel;

public:
    Challenge(GameInfo & info, Car & targetCar);

    void step();
    void render(Renderer & renderer);
};