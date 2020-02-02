#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "mechanics/drive.h"
#include <memory>
#include "maneuvers/recovery.h"


Recovery::Recovery(GameInfo & info) : Maneuver(info), reorient(info.myCar)
{
}

void Recovery::step()
{
    reorient.target_orientation = look_at(xy(info.myCar.velocity));
    reorient.step(info.dt);
    controls = reorient.controls;

    finished = info.myCar.on_ground;
}