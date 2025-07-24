#ifndef PHYSICS_ENGINE_7_HPP
#define PHYSICS_ENGINE_7_HPP

#include "rigidbody.hpp"
#include "commonPhysics.hpp"

#include "math.hpp"

#include "cmap_flatgrass.hpp"

class PhysicsEngine
{
    PhysicsEngineState state;
    CMap* collisionsMap = nullptr;
public:
    PhysicsEngine()
    {
        for (size_t i = 0; i < MAXRIGIDBODIES; i++)
        {
            rigidbodies[i].used = false;
        }

        // assign collision map
        collisionsMap = new cmap_Flatgrass();

        state = PE_RUNNING;
    };

    Rigidbody rigidbodies[MAXRIGIDBODIES];

    void initOBBRigidbody(u8 id, vector3 position, vector3 size, int32 mass);
    void initSphereRigidbody(u8 id, vector3 position, int32 radius, int32 mass);
    void killRigidbody(u8 id);
    void applyForce(u8 id, vector3 force);

    void simulate();
    
    vector3 sphereCollideAndSlide(vector3 velocity, vector3 position, int32 radius, int32 depth);

    const PhysicsEngineState getState() { return state; }
};

#endif