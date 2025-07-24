#ifndef BSP_HPP
#define BSP_HPP

#include <nds.h>
#include "math.hpp"

// possible leaf states
enum BSPLeafState
{
    UNASSIGNED,
    SOLID,
    AIR
};

// binary space partitioning tree node
class BSPNode
{
public:
    vector3 planeN;
    int32 planeD;

    BSPLeafState state = UNASSIGNED;

    BSPNode* frontNode = nullptr;
    BSPNode* backNode = nullptr;

    bool isLeaf() { return frontNode == nullptr && backNode == nullptr; }
};

int32 planeSignedDistance(vector3 p, vector3 n, int32 d);
BSPLeafState traversePoint(BSPNode* root, vector3 point);
bool recursiveLineTrace(BSPNode* node, const vector3 start, const vector3 end, vector3& intersection, vector3& normal, int32 offset = 0);
bool sphereRecursiveLineTrace(BSPNode* node, const vector3 start, const vector3 end, vector3& intersection, vector3& normal, int32 radius);

#endif