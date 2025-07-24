#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <NEMain.h>
#include "gameObject.hpp"

#include "math.hpp"

#include "cmap.hpp"
#include "tool.hpp"
#include "props.hpp"

struct PlayerInput
{
    vector3 wishDir;
    bool wishJump;
    bool wishPrimary;
    bool look;
    vector2 lookDelta;
};

/// the player class should
/// - handle the camera
/// - update the player entity with input
class Player : public GameObject
{
    NE_Camera& camera;
    vector3 cameraLocal = vector3_zero;

    int32 walkingSpeed = 2;
    vector3 playerVelocity = vector3_zero;
    bool grounded = false;

    const int32 _skinWidth = 200;

    GameProp* targetProp = nullptr; // target prop the player is looking
    vector3 cameraForward;          // camera forward dir

public:
    Tool* toolHeld = nullptr;       // tool currently held by the player
    CMap& collisionsMap;

    Player(NE_Camera& camera, CMap& map) : camera(camera), collisionsMap(map) {};
    
    ~Player()
    {
        toolHeld = nullptr;
    };

    void updateWithInput(PlayerInput& input);
    void getTargetPropLineTrace(GameProp** props);
    void animateTool(int32 angle, int32 lookX, int32 lookY);
    int32 viewbobbing(int32 angle);
    vector3 collideAndSlide(vector3 velocity, vector3 position, int32 depth);
    bool checkGrounded();
};

#endif