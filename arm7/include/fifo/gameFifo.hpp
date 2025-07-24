#ifndef GAME_FIFO_7_HPP
#define GAME_FIFO_7_HPP

#include <nds.h>
#include "fifoCommands.hpp"

#include "physicsEngine.hpp"
#include "rigidbody.hpp"

#include "commonPhysics.hpp"

static void syncPhysicsEngine(const PhysicsEngine& engine)
{
    for (size_t i = 0; i < MAXRIGIDBODIES; i++)
    {
        if (engine.rigidbodies[i].used)
        {
            if (!engine.rigidbodies[i].isSphere)
            {
                const Rigidbody* rb = &engine.rigidbodies[i];
                fifoSendValue32(FIFO_USER_01, i);
                fifoSendValue32(FIFO_USER_02, rb->obb.position.x);
                fifoSendValue32(FIFO_USER_03, rb->obb.position.y);
                fifoSendValue32(FIFO_USER_04, rb->obb.position.z);
                // int32 to u16
                // rounding with +4096, then shifting to compress the 32 bits in 16 (we are losing integer bits but retaining full floating precision)
                fifoSendValue32(FIFO_USER_05,((rb->obb.transformationMatrix[3]+4096)<<16)|((u16)rb->obb.transformationMatrix[0]+4096));
                fifoSendValue32(FIFO_USER_06,((rb->obb.transformationMatrix[1]+4096)<<16)|((u16)rb->obb.transformationMatrix[6]+4096));
                fifoSendValue32(FIFO_USER_07,((rb->obb.transformationMatrix[7]+4096)<<16)|((u16)rb->obb.transformationMatrix[4]+4096));
            }
            else
            {
                const Rigidbody* rb = &engine.rigidbodies[i];
                fifoSendValue32(FIFO_USER_01, i);
                fifoSendValue32(FIFO_USER_02, rb->obb.position.x);
                fifoSendValue32(FIFO_USER_03, rb->obb.position.y);
                fifoSendValue32(FIFO_USER_04, rb->obb.position.z);
            }
        }
    }
}

// still missing implementations for the managing requests because im lazy. namely:
// - PER_START = 1,
// - PER_STOP = 2,
// - PER_RESET = 3,
// these can be found in fifoCommands.hpp in common/
static void listenPhysicsEngineRequests(PhysicsEngine& engine)
{
    // channel 8 is reserved to ARM9 -> ARM7 comms
	while (fifoCheckValue32(FIFO_USER_08))
	{
        u32 data = fifoGetValue32(FIFO_USER_08);
        u8 id = data >> PERLENGTH;

        switch(data & PERMASK)    // get instruction
        {
            case PER_INITOBBRIGIDBODY:
                {
                    int32 mass = 1;
                    vector3 position, size;
                    // get position
                    while(!fifoCheckValue32(FIFO_USER_08));
						position.x = fifoGetValue32(FIFO_USER_08);
					while(!fifoCheckValue32(FIFO_USER_08));
						position.y = fifoGetValue32(FIFO_USER_08);
					while(!fifoCheckValue32(FIFO_USER_08));
						position.z = fifoGetValue32(FIFO_USER_08);
                    // get size
                    while(!fifoCheckValue32(FIFO_USER_08));
						u32 x = fifoGetValue32(FIFO_USER_08);
						size.x = x & ((1<<16) - 1);
						size.y = (x>>16) & ((1<<16) - 1);
                    // get mass
                    while(!fifoCheckValue32(FIFO_USER_08));
                        mass = fifoGetValue32(FIFO_USER_08);
                    
                    engine.initOBBRigidbody(id, position, size, mass);
                }
                break;
            
            case PER_INITSPHERERIGIDBODY:
                {
                    int32 mass = 1;
                    vector3 position;
                    int32 radius = 1;

                    // get position
                    while(!fifoCheckValue32(FIFO_USER_08));
						position.x = fifoGetValue32(FIFO_USER_08);
					while(!fifoCheckValue32(FIFO_USER_08));
						position.y = fifoGetValue32(FIFO_USER_08);
					while(!fifoCheckValue32(FIFO_USER_08));
						position.z = fifoGetValue32(FIFO_USER_08);
                    // get radius
                    while(!fifoCheckValue32(FIFO_USER_08));
						radius = fifoGetValue32(FIFO_USER_08);
                    // get mass
                    while(!fifoCheckValue32(FIFO_USER_08));
                        mass = fifoGetValue32(FIFO_USER_08);
                    
                    engine.initSphereRigidbody(id, position, radius, mass);
                }
                break;

            case PER_KILLRIGIDBODY:
                {
                    engine.killRigidbody(id);
                }
                break;
            
            case PER_APPLYFORCE:
                {
                    vector3 force;

                    // get force
                    while(!fifoCheckValue32(FIFO_USER_08));
						force.x = fifoGetValue32(FIFO_USER_08);
					while(!fifoCheckValue32(FIFO_USER_08));
						force.y = fifoGetValue32(FIFO_USER_08);
					while(!fifoCheckValue32(FIFO_USER_08));
						force.z = fifoGetValue32(FIFO_USER_08);
                    
                    engine.applyForce(id, force);
                }
                break;
        }
    }
}

#endif