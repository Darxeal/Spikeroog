#include "maneuvers/maneuver.h"
#include "mechanics/travel.h"
#include "utils/vector_math.h"
#include "utils/misc.h"
#include "simulation/ball.h"
#include "simulation/field.h"
#include "maneuvers/collect_boost.h"

CollectBoost::CollectBoost(GameInfo & info) : Maneuver(info), travel(info.myCar)
{
    std::vector<vec3> padPositions(6);
    for (size_t i = 0; i < 6; i++)
    {
        padPositions[i] = info.largeBoostPads[i]->position;
    }

    travel.target = nearest_point(info.myCar.position, padPositions);
};

void CollectBoost::step()
{
    travel.step(info.dt);
    controls = travel.controls;
    finished = ground_distance(info.myCar.position, travel.target) < 50.0f or info.myCar.boost == 100;
}

void CollectBoost::render(Renderer & renderer)
{
    travel.render(renderer);
}