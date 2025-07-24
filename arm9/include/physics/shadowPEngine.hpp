#ifndef SHADOW_PEngine_9_HPP
#define SHADOW_PEngine_9_HPP

#include <nds.h>
#include "commonPhysics.hpp"

#include "math.hpp"
#include "gameFifo.hpp"

// to do:
// create a rigidbody component class for gameObjects
// we need to pass those to the physicsEngine

// shadow rigidbody
struct SRigidbody
{
    u8 id;
    bool used;
    bool isSphere;

    int32 mass;

    vector3 position;
    int32 transformationMatrix[9];
};


class ShadowPEngine
{
    PhysicsEngineState state;

    SRigidbody rigidbodies[MAXRIGIDBODIES];

public:
    ShadowPEngine()
    {
        for (size_t i = 0; i < MAXRIGIDBODIES; i++)
        {
            rigidbodies[i].id = i;
            rigidbodies[i].used = false;
        }
        
        state = PE_RUNNING;
    };

    // user requested
    SRigidbody* initOBBRigidbody(vector3 position, vector3 size, int32 mass = 1)
    {
        u8 id;
        for (id = 0; id < MAXRIGIDBODIES; id++)
        {
            if (!rigidbodies[id].used)
            {
                SRigidbody* rb = &rigidbodies[id];
                
                rigidbodies[id].mass = mass;
                rigidbodies[id].position = position;
                rigidbodies[id].used = true;
                
                rigidbodies[id].isSphere = false;

                // call the physics engine through gameFifo
                requestInitOBBRigidbody(id, position, size, mass);

                return rb;
            }
        }

        return nullptr;
    };

    // user requested
    SRigidbody* initSphereRigidbody(vector3 position, int32 radius, int32 mass = 1)
    {
        u8 id;
        for (id = 0; id < MAXRIGIDBODIES; id++)
        {
            if (!rigidbodies[id].used)
            {
                SRigidbody* rb = &rigidbodies[id];
                
                rigidbodies[id].mass = mass;
                rigidbodies[id].position = position;
                rigidbodies[id].used = true;
                
                rigidbodies[id].isSphere = true;

                // call the physics engine through gameFifo
                requestInitSphereRigidbody(id, position, radius, mass);

                return rb;
            }
        }

        return nullptr;
    };

    // user requested
    void killRigidbody(const SRigidbody* rb)
    {
        if (rb->id >= MAXRIGIDBODIES)
            return;
    
        rigidbodies[rb->id].used = false;

        // call the physics engine through gameFifo
        requestKillRigidbody(rb->id);
    };

    // user requested
    void applyForce(const SRigidbody* rb, vector3 force)
    {
        if (rb->id >= MAXRIGIDBODIES && !rb->used)
            return;

        // call the physics engine through gameFifo
        requestApplyForce(rb->id, force);
    };

    // called by gameFifo
    void updateOBBRigidbody(u8 id, vector3 position, int32 transformationMatrix[9])
    {
        if (id >= MAXRIGIDBODIES)
            return;
        
        rigidbodies[id].position = position;
        for (size_t i = 0; i < 9; i++)
        {
            rigidbodies[id].transformationMatrix[i] = transformationMatrix[i];
        }  
    };

    // called by gameFifo
    void updateSphereRigidbody(u8 id, vector3 position)
    {
        if (id >= MAXRIGIDBODIES)
            return;
        
        rigidbodies[id].position = position;
    };
    
    void syncShadowPhysicsEngine()
    {
        while (fifoCheckValue32(FIFO_USER_01))
        {
            const int id = fifoGetValue32(FIFO_USER_01);
            while(!fifoCheckValue32(FIFO_USER_02));
            while(!fifoCheckValue32(FIFO_USER_03));
            while(!fifoCheckValue32(FIFO_USER_04));

            if(id < MAXRIGIDBODIES)
            {
                if (!rigidbodies[id].isSphere)
                {
                    vector3 position;
                    int32 transformationMatrix[9];

                    position.x = fifoGetValue32(FIFO_USER_02);
                    position.y = fifoGetValue32(FIFO_USER_03);
                    position.z = fifoGetValue32(FIFO_USER_04);
                    
                    // wait for other OBB values
                    while(!fifoCheckValue32(FIFO_USER_05));
                    while(!fifoCheckValue32(FIFO_USER_06));
                    while(!fifoCheckValue32(FIFO_USER_07));
                    
                    int32 x = fifoGetValue32(FIFO_USER_05);
                    transformationMatrix[0] = (s16)x-4096;
                    transformationMatrix[3] = (s16)(x>>16)-4096;
                    x = fifoGetValue32(FIFO_USER_06);
                    transformationMatrix[6] = (s16)(x)-4096;				
                    transformationMatrix[1] = (s16)(x>>16)-4096;
                    x = fifoGetValue32(FIFO_USER_07);
                    transformationMatrix[4] = (s16)x-4096;
                    transformationMatrix[7] = (s16)(x>>16)-4096;
                    
                    // reconstruct the third normal we didnt send over with a cross product
                    vector3 v1 = vector3(transformationMatrix[0], transformationMatrix[3], transformationMatrix[6]);
                    vector3 v2 = vector3(transformationMatrix[1], transformationMatrix[4], transformationMatrix[7]);
                    //vector3 v = normalize(vectorProduct(v1, v2));
                    vector3 v;
                    crossf32(&v1.x, &v2.x, &v.x);
                    normalizef32(&v.x);

                    transformationMatrix[2] = v.x;
                    transformationMatrix[5] = v.y;
                    transformationMatrix[8] = v.z;

                    updateOBBRigidbody(id, position, transformationMatrix);
                }
                else
                {
                    vector3 position;

                    position.x = fifoGetValue32(FIFO_USER_02);
                    position.y = fifoGetValue32(FIFO_USER_03);
                    position.z = fifoGetValue32(FIFO_USER_04);

                    updateSphereRigidbody(id, position);
                }
            }
        }
    };

    const PhysicsEngineState getState() { return state; }
};

#endif