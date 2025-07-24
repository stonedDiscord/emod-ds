/*
#ifndef GAMEOBJECT_COMPONENT_HPP
#define GAMEOBJECT_COMPONENT_HPP

#include <nds.h>
#include "gameObject.hpp"

#include "math.hpp"

class Component
{
public:
    GameObject& obj;

    Component(GameObject& obj) : obj(obj) {};
};

// shadow of the actual rigidbody on the physics engine
// allows to control that instance, so only controllable fields are available (non simulated stuff)
class C_Rigidbody : public Component
{
public:
    int32 mass;
    vector3 size;

    C_Rigidbody(GameObject& obj) : Component(obj)
    {

    };
};

#endif
*/