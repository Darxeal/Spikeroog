#pragma once
#include "maneuvers/maneuver.h"
#include "linear_algebra/math.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "mechanics/dodge.h"
#include "mechanics/drive.h"
#include <memory>

#include "maneuvers/spike_ceiling_shot.h"



SpikeCeilingShot::SpikeCeilingShot(GameInfo & info) : Maneuver(info), travel(info.myCar), dodge(info.myCar), drive(info.myCar)
{
    travel.target = nearest_point(info.myCar.position + info.myCar.velocity, {vec3{4100,0,2000}, vec3{-4100,0,2000}}); // pick nearest wall
    travel.allowDodges = false; // only wavedash with spikes

    drive.speed = 2300.0f;
    drive.target = info.theirGoal + vec3{0,0,3000};

    dodge.jump_duration = 0.0f;
    dodge.delay = 0.0f;
}

void SpikeCeilingShot::step()
{
    switch (phase)
    {
        case 0: // drive up the wall
            travel.step(info.dt);
            controls = travel.controls;
            if (info.myCar.position[2] > 1500.0f)
                phase++;
        break;

        case 1: // drive on the ceiling towards their goal
            drive.step(info.dt);
            controls = drive.controls;
            if (not info.myCar.on_ground)
                phase++;
        break;

        case 2: // fall
            if (info.myCar.position[2] < 1100.0f)
                phase++;
        break;

        case 3: // flick
            dodge.direction = direction(info.myCar.position, info.theirGoal);
            dodge.step(info.dt);
            controls = dodge.controls;
            if (dodge.timer > 0.1f)
                controls.useItem = true;
        break;
    }
    finished = !isBallAttached(info.myCar, info) or dodge.finished;
}