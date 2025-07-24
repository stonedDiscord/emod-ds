#ifndef RIGIDBODY_7_HPP
#define RIGIDBODY_7_HPP

#include <nds.h>
#include "math.hpp"
#include "OBB.hpp"

class Rigidbody
{
public:
    bool used;
    bool isSphere = false;
    int32 radius;

    int32 mass;
    vector3 velocity, angularVelocity, momentum, angularMomentum, forces;

    bool useGravity;
    bool isKinematic;

    OBBStruct obb;

    Rigidbody(int32 m = 1)
    {
        used = false;

        mass = m;
        useGravity = true;
        isKinematic = false;
    };
};

#endif