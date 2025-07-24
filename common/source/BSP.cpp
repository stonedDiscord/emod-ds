#include "BSP.hpp"

int32 planeSignedDistance(const vector3 p, const vector3 n, const int32 d)
{
    return dotProduct(n, p) + d;
};

BSPLeafState traversePoint(BSPNode* root, vector3 point)
{
    BSPNode* node = root;

    while (!node->isLeaf())
    {
        int32 d = planeSignedDistance(point, node->planeN, node->planeD);

        if (d >= 0)
            node = node->frontNode;
        else
            node = node->backNode;
    }

    return node->state;
};

bool recursiveLineTrace(BSPNode* node, const vector3 start, const vector3 end, vector3& intersection, vector3& normal, int32 offset)
{
    // handle leaves
    if (node->isLeaf())
    {
        if (node->state == SOLID)
        {
            intersection = start;
            
            return true;
        }
        intersection = vector3_zero;
        return false;
    }

    // distances
    int32 t1 = planeSignedDistance(start, node->planeN, node->planeD - offset);
    int32 t2 = planeSignedDistance(end, node->planeN, node->planeD - offset);

    // the line lies entirely within one of the two subspaces
    if (t1 >= 0 && t2 >= 0)
        return recursiveLineTrace(node->frontNode, start, end, intersection, normal, offset);
    if (t1 < 0 && t2 < 0)
        return recursiveLineTrace(node->backNode, start, end, intersection, normal, offset);

    // line crosses different nodes
    int32 denom = (t1 - t2);
    int32 frac = t1 / denom;    // intersection with the split plane
    frac = CLAMP(frac, 0, 4096);

    // vector3 mid = p1 + frac * (p2 - p1);
    vector3 mid = addVector(start, multVector(vectorDifference(end, start), frac));
    bool keepBack = t1 < 0;

    if (!keepBack)
    {
        normal = node->planeN;
    }

    // split the problem
    if (recursiveLineTrace(keepBack ? node->backNode : node->frontNode, start, mid, intersection, normal, offset))
        return true;
    return recursiveLineTrace(keepBack ? node->frontNode : node->backNode, mid, end, intersection, normal, offset);
}

bool sphereRecursiveLineTrace(BSPNode* node, const vector3 start, const vector3 end, vector3& intersection, vector3& normal, int32 radius)
{
    // handle leaves
    if (node->isLeaf())
    {
        if (node->state == SOLID)
        {
            intersection = start;
            
            return true;
        }
        intersection = vector3_zero;
        return false;
    }

    // distances
    int32 t1 = planeSignedDistance(start, node->planeN, node->planeD) - radius;
    int32 t2 = planeSignedDistance(end, node->planeN, node->planeD) - radius;

    // the line lies entirely within one of the two subspaces
    if (t1 >= 0 && t2 >= 0)
        return recursiveLineTrace(node->frontNode, start, end, intersection, normal, radius);
    if (t1 < 0 && t2 < 0)
        return recursiveLineTrace(node->backNode, start, end, intersection, normal, radius);

    // line crosses different nodes
    int32 denom = (t1 - t2);
    int32 frac = t1 / denom;    // intersection with the split plane
    frac = CLAMP(frac, 0, 4096);

    // vector3 mid = p1 + frac * (p2 - p1);
    vector3 mid = addVector(start, multVector(vectorDifference(end, start), frac));
    bool keepBack = t1 < 0;

    if (!keepBack)
    {
        normal = node->planeN;
    }

    // split the problem
    if (recursiveLineTrace(keepBack ? node->backNode : node->frontNode, start, mid, intersection, normal, radius))
        return true;
    return recursiveLineTrace(keepBack ? node->frontNode : node->backNode, mid, end, intersection, normal, radius);
}

