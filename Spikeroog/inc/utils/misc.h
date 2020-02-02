#pragma once
#include "linear_algebra/math.h"
#include "linear_algebra/vec.h"
#include "linear_algebra/mat.h"
#include "simulation/car.h"
#include "simulation/ball.h"

#include "utils/vector_math.h"

inline bool isBallAttached(Car & car, GameInfo & info)
{
    return distance(info.ball.position, car.position) < 200.0f;
}

