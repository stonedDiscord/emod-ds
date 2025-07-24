#ifndef STATIC_PHYSICS_HPP
#define STATIC_PHYSICS_HPP

#include <nds.h>
#include "shadowPEngine.hpp"

struct StaticPhysics
{
    static ShadowPEngine* enginePointer;
    static bool engineActive() { return enginePointer != nullptr; }
};

#endif