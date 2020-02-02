#pragma once
#include "mechanics/drive.h"
#include "mechanics/dodge.h"
#include "mechanics/wavedash.h"
#include <memory>

class Travel
{

    float timeSpentOnGround = 0.0f;
    bool dodging = false;
    bool wavedashing = false;

    const float dodgeDuration = 1.6f;
    const float wavedashDuration = 0.8f;
    const float groundRestTime = 0.2f;

    Drive drive;

    std::unique_ptr<Dodge> dodge;
    std::unique_ptr<Wavedash> wavedash;

public:
    Input controls;
    Car & car;
    
    vec3 target;

    bool allowDodges = true;
    bool allowWavedashes = true;

    float noActionDistance = 0.0f;

    Travel(Car & car);

    float estimateMaxSpeed(float timeLeft);

    void step(float dt);
    void render(Renderer & renderer);
};
    
