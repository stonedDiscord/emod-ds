#include "physicsEngine.hpp"

vector3 worldUpVec = vector3(0, floattof32(1), 0);
vector3 worldDownVec = vector3(0, floattof32(-1), 0);

void PhysicsEngine::initOBBRigidbody(u8 id, vector3 position, vector3 size, int32 mass)
{
    if (id >= MAXRIGIDBODIES)
        return;
    
    rigidbodies[id].isSphere = false;
    rigidbodies[id].obb.position = position;
    rigidbodies[id].obb.size = size;
    
    rigidbodies[id].mass = mass;
    rigidbodies[id].velocity = vector3_zero;
    rigidbodies[id].angularVelocity = vector3_zero;
    rigidbodies[id].momentum = vector3_zero;
    rigidbodies[id].angularMomentum = vector3_zero;
    rigidbodies[id].forces = vector3_zero;

    rigidbodies[id].useGravity = true;
    rigidbodies[id].isKinematic = false;

    rigidbodies[id].used = true;
};

void PhysicsEngine::initSphereRigidbody(u8 id, vector3 position, int32 radius, int32 mass)
{
    if (id >= MAXRIGIDBODIES)
        return;
    
    rigidbodies[id].isSphere = true;
    rigidbodies[id].obb.position = position;
    rigidbodies[id].radius = radius;
    
    rigidbodies[id].mass = mass;
    rigidbodies[id].velocity = vector3_zero;
    rigidbodies[id].angularVelocity = vector3_zero;
    rigidbodies[id].momentum = vector3_zero;
    rigidbodies[id].angularMomentum = vector3_zero;
    rigidbodies[id].forces = vector3_zero;

    rigidbodies[id].useGravity = true;
    rigidbodies[id].isKinematic = false;

    rigidbodies[id].used = true;
};

void PhysicsEngine::killRigidbody(u8 id)
{
    if (id >= MAXRIGIDBODIES)
        return;
    
    rigidbodies[id].used = false;
};

void PhysicsEngine::applyForce(u8 id, vector3 force)
{
    if (rigidbodies[id].used)
    {
        rigidbodies[id].forces = addVector(rigidbodies[id].forces, force);
    }
};

// simulation codeeeee
void PhysicsEngine::simulate()
{
    // fuck
    if (collisionsMap == nullptr)
        return;

    for (int i = 0; i < MAXRIGIDBODIES; i++)
    {
        if (rigidbodies[i].used)
        {
            Rigidbody* rb = &rigidbodies[i];
            if (rb->isSphere)
            {
                // forces
                rb->velocity = addVector(rb->velocity, multVector16(rb->forces, fixedDelta));
                rb->forces = vector3_zero;

                // collide against other spheres
                for (int j = 0; j < MAXRIGIDBODIES; j++)
                {
                    if (rigidbodies[j].used && i != j)
                    {
                        Rigidbody* otherRb = &rigidbodies[j];
                        if (otherRb->isSphere)
                        {
                            // fixed int32 overflow
                            vector3 to = vectorDifference(otherRb->obb.position, rb->obb.position);
                            int32 sum = rb->radius + otherRb->radius;
                            if (ABS(to.x) < sum && ABS(to.y) < sum && ABS(to.z) < sum)
                            {
                                if (magnitude(to) < sum)
                                    applyForce(j, multVectorInt(to, 10));
                            }
                        }
                    }
                }

                // fuck yes spheres physics sim
                rb->velocity.y -= mulv16(floattof32(9), fixedDelta); // gravity

                // if grounded
                vector3 hit, normal;
                if (recursiveLineTrace(collisionsMap->root, rb->obb.position, addVector(rb->obb.position, multVector16(worldDownVec, rb->radius + 1000)), hit, normal, 0))
                {
                    // friction
                    rb->velocity = multVector(rb->velocity, floattof32(0.8));
                }
                else
                {
                    // air friction
                    rb->velocity = multVector(rb->velocity, floattof32(0.9));
                }
                
                // collide and slide
                rb->obb.position = addVector(rb->obb.position, sphereCollideAndSlide(multVector(rb->velocity, fixedDelta), rb->obb.position, rb->radius, 0));
            }
        }
    }
};

const int32 _skinWidth = 200;
vector3 PhysicsEngine::sphereCollideAndSlide(vector3 velocity, vector3 position, int32 radius, int32 depth)
{
    if (depth >= 2)
        return vector3_zero;
    
    int32 dist = magnitude(velocity) + _skinWidth;
    vector3 dir = normalize(velocity);

    vector3 hit, normal = vector3_zero;
    if (sphereRecursiveLineTrace(collisionsMap->root, position, addVector(position, multVector(dir, dist)), hit, normal, radius))
    {
        if (normal.x + normal.y + normal.z == 0)
            return velocity;
        
        //int32 hitDistance = magnitude(vectorDifference(hit, position)) - 1000; // cause of bug
        int32 hitDistance = CLAMP(magnitude(vectorDifference(hit, position)), 0, radius);
        vector3 snapToSurface = multVector(dir, hitDistance - _skinWidth);
        vector3 leftover = vectorDifference(velocity, snapToSurface);
        
        if (magnitude(snapToSurface) <= _skinWidth)
            snapToSurface = vector3_zero;

        projectVectorPlane(&leftover, normal);

        return sphereCollideAndSlide(leftover, position, radius, depth+1);
    }

    return velocity;
};
