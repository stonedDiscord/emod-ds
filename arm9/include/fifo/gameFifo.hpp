#ifndef GAME_FIFO_9_HPP
#define GAME_FIFO_9_HPP

#include <nds.h>
#include "fifoCommands.hpp"

#include "commonPhysics.hpp"
#include "math.hpp"

static void requestInitOBBRigidbody(u8 id, vector3 position, vector3 size, int32 mass)
{
	fifoSendValue32(FIFO_USER_08, PER_INITOBBRIGIDBODY | ((id) << PERLENGTH));
	
    fifoSendValue32(FIFO_USER_08, (position.x));
	fifoSendValue32(FIFO_USER_08, (position.y));
	fifoSendValue32(FIFO_USER_08, (position.z));
    fifoSendValue32(FIFO_USER_08, (size.x & ((1<<16)-1)) | ((size.y & ((1<<16)-1)) << 16));	
    fifoSendValue32(FIFO_USER_08, mass);
}

static void requestInitSphereRigidbody(u8 id, vector3 position, int32 radius, int32 mass)
{
	fifoSendValue32(FIFO_USER_08, PER_INITSPHERERIGIDBODY | ((id) << PERLENGTH));
	
    fifoSendValue32(FIFO_USER_08, (position.x));
	fifoSendValue32(FIFO_USER_08, (position.y));
	fifoSendValue32(FIFO_USER_08, (position.z));
    fifoSendValue32(FIFO_USER_08, radius);	
    fifoSendValue32(FIFO_USER_08, mass);
}

static void requestKillRigidbody(u8 id)
{
	fifoSendValue32(FIFO_USER_08, PER_KILLRIGIDBODY | ((id) << PERLENGTH));
}

static void requestApplyForce(u8 id, vector3 force)
{
	fifoSendValue32(FIFO_USER_08, PER_APPLYFORCE | ((id) << PERLENGTH));
	
    fifoSendValue32(FIFO_USER_08, (force.x));
	fifoSendValue32(FIFO_USER_08, (force.y));
	fifoSendValue32(FIFO_USER_08, (force.z));
}

#endif