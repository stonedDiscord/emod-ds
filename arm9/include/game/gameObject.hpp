#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <nds.h>
#include <vector>

#include "math.hpp"

using namespace std;

class GameObject
{
protected:
    vector3 position = vector3_zero;
public:
    vector3 getPosition() const { return position; }
    
    virtual void setPositionI(int32 x, int32 y, int32 z)
    { 
        position.x = x;
        position.y = y;
        position.z = z;
    }
    #define setPosition(x, y, z) \
        setPositionI(floattof32(x), floattof32(y), floattof32(z))
};

#endif