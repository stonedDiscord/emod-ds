#include "player.hpp"

#include <NEMain.h>
#include "staticPhysics.hpp"
#include "audioManager.hpp"

int32 worldUp[3] = {0, floattof32(1), 0};
vector3 worldUpVec = vector3(0, floattof32(1), 0);
vector3 worldDownVec = vector3(0, floattof32(-1), 0);

const int32 playerExtent = floattof32(1.5);

int32 yCameraTarget = 0;
s16 viewbobbingAngle = 0;
bool lastGrounded = false;

GameProp* lockedProp = nullptr;

void Player::updateWithInput(PlayerInput& input)
{
    // TO DO: clean up all of this mess
    
    if (input.look)
        NE_CameraRotateFree(&camera, input.lookDelta.y, input.lookDelta.x, 0);

    /* CAMERA UNIT VECTORS */
    int32 f[3], r[3], u[3];
    for (int i = 0; i < 3; i++)
        f[i] = camera.to[i] - camera.from[i];   // camera front
    
    // quick clamp camera up and down
    normalizef32((int32*) &f);
    f[1] = CLAMP(f[1], -3000, 3000);

    crossf32(f, worldUp, r);                    // camera right
    crossf32(r, f, u);                          // camera up

    cameraForward = vector3(f[0], f[1], f[2]);
    normalizef32(&cameraForward.x);
    vector3 cameraRight = vector3(r[0], r[1], r[2]);
    vector3 cameraUp = vector3(u[0], u[1], u[2]);

    /* GET ORIENTATION */
    vector3 orientationForward = cameraForward;
    projectVectorPlane(&orientationForward, worldUpVec);
    normalizef32(&orientationForward.x);
    vector3 orientationRight = vector3_zero;
    crossf32(&orientationForward.x, worldUp, &orientationRight.x);

    /* UPDATE PLAYER VELOCITY */
    // update ground status
    lastGrounded = grounded;
    grounded = checkGrounded(); printf("grounded %d\n", grounded);

    // wish speed calculation
    vector3 wishSpeed = vector3_zero;
    wishSpeed = multVector(orientationForward, input.wishDir.z * walkingSpeed);
    wishSpeed = addVector(wishSpeed, multVector(orientationRight, input.wishDir.x * walkingSpeed));
    
    playerVelocity = addVector(playerVelocity, wishSpeed);
    playerVelocity = multVector(playerVelocity, grounded ? floattof32(0.7) : floattof32(0.8));  // some friction
    
    // ground planar velocity
    vector3 playerVelocityPlanar = playerVelocity; projectVectorPlane(&playerVelocityPlanar, worldUpVec);
    int32 magPlanar = magnitude(playerVelocityPlanar); printf("playerVelMag %d\n", magPlanar);

    if (grounded && magPlanar > floattof32(1))
    {
        viewbobbingAngle += 10;

        if (viewbobbingAngle % 360 >= 350)
            AudioManager::playFootstep();

        if(viewbobbingAngle >= 360 * 2)
        {
            viewbobbingAngle = 0;
        }
    }
    else
    {
        viewbobbingAngle = 0;
    }

    // apply gravity and jumping
    playerVelocity.y -= mulf32(floattof32(30), fixedDelta); // gravity
    if (grounded && input.wishJump)
        playerVelocity.y = floattof32(15);

    /* UPDATE PLAYER POSITION */
    position = addVector(position, collideAndSlide(multVector(playerVelocity, fixedDelta), position, 0));

    /* UPDATE CAMERA */
    camera.matrix_is_updated = false;

    // camera Y effects
    yCameraTarget = lerpInt(yCameraTarget, position.y - cameraLocal.y, fixedDelta * 4);
    if (!lastGrounded && grounded)
    {
        yCameraTarget += playerVelocity.y;
        AudioManager::playFootstep();
    }

    yCameraTarget += viewbobbing(viewbobbingAngle);

    cameraLocal.y = lerpInt(cameraLocal.y, yCameraTarget, fixedDelta);

    camera.from[0] = position.x + cameraLocal.x;
    camera.from[1] = position.y + cameraLocal.y;
    camera.from[2] = position.z + cameraLocal.z;
    
    vector3 to = addVector(addVector(cameraLocal, position), multVector(cameraForward, floattof32(5)));
    camera.to[0] = to.x;
    camera.to[1] = to.y;
    camera.to[2] = to.z;

    /* TOOL UPDATE */
    if (toolHeld != nullptr)
    {
        animateTool(viewbobbingAngle, input.lookDelta.x, input.lookDelta.y);

        if (input.wishPrimary)
        {
            if (lockedProp == nullptr)
                lockedProp = targetProp;

            ToolContext context = {
                position,
                cameraForward,
                lockedProp
            };

            if (lockedProp != nullptr)
                lockedProp->outlined = true;

            toolHeld->primaryUse(context);
        }
        else
        {
            // simple locking on props
            if (lockedProp != nullptr)
            {
                lockedProp->outlined = false;
                lockedProp = nullptr;
            }
        }
    }
};

vector3 toolWalkingOffset = vector3_zero;
vector3 toolLookOffset = vector3_zero;
void Player::animateTool(int32 angle, int32 lookX, int32 lookY)
{
    toolWalkingOffset = vector3(
        mulf32(cosLerp(degreesToAngle(angle / 2)), 20),
        mulf32(ABS(sinLerp(degreesToAngle(angle / 2))), -15),
        0
    );

    toolLookOffset.x = lerpInt(toolLookOffset.x, -lookX * 30, fixedDelta * 2);
    toolLookOffset.y = lerpInt(toolLookOffset.y, CLAMP(-lookY * 10, -100, 100), fixedDelta * 2);
    toolLookOffset = lerpVector3(toolLookOffset, vector3_zero, fixedDelta * 3);

    toolHeld->localPosition = lerpVector3(toolHeld->localPosition, addVector(toolWalkingOffset, toolLookOffset), fixedDelta * 3);
};

int32 Player::viewbobbing(int32 angle)
{
    return mulf32(sinLerp(degreesToAngle(angle)), floattof32(0.2));
};

vector3 Player::collideAndSlide(vector3 velocity, vector3 position, int32 depth)
{
    if (depth >= 5)
        return vector3_zero;
    
    int32 dist = magnitude(velocity) + _skinWidth;
    vector3 dir = velocity; normalizef32(&dir.x);

    vector3 hit, normal = vector3_zero;
    if (recursiveLineTrace(collisionsMap.root, position, addVector(position, multVector(dir, dist)), hit, normal, playerExtent))
    {
        if (normal.x + normal.y + normal.z == 0)
            return velocity;

        printf("[depth %d] normal x %d y %d z %d", depth, normal.x, normal.y, normal.z);

        int32 hitDistance = magnitude(vectorDifference(hit, position));
        vector3 snapToSurface = multVector(dir, hitDistance - _skinWidth);
        vector3 leftover = vectorDifference(velocity, snapToSurface);

        if (magnitude(snapToSurface) <= _skinWidth)
            snapToSurface = vector3_zero;
        
        projectVectorPlane(&leftover, normal);

        return addVector(snapToSurface, collideAndSlide(leftover, addVector(position, snapToSurface), depth+1));
    }

    return velocity;
};

bool Player::checkGrounded()
{
    vector3 hit, normal = vector3_zero;
    return recursiveLineTrace(collisionsMap.root, position, addVector(position, multVector(worldDownVec, playerExtent + 1000)), hit, normal, 0);
};

void Player::getTargetPropLineTrace(GameProp** props)
{
    int32 mapDist = floattof32(6);
    
    vector3 hit, normal = vector3_zero;
    if(recursiveLineTrace(collisionsMap.root, position, addVector(position, multVector(cameraForward, mapDist)), hit, normal, 0))
        mapDist = magnitude(vectorDifference(hit, position));
    
    int32 minDist = mapDist;
    targetProp = nullptr;
    for (int i = 0; i < MAXRIGIDBODIES; i++)
    {
        GameProp* prop = props[i];
        if (prop != nullptr)
        {
            //fprintf(stderr, "map dist %d, prop dist %d\n", mapDist, prop->distance);
            if (prop->distance > minDist && prop->distance < floattof32(18))
            {
                minDist = prop->distance;
                targetProp = prop;
            }
        }
    }
};