#ifndef OBB_HPP
#define OBB_HPP

#define NUMOBBSEGMENTS (12)

#include <nds.h>
#include "math.hpp"

// how the edges (segments) in an OBB connect from one vertex to another
static const u8 OBBSegments[NUMOBBSEGMENTS][2] =
	{{0, 1}, {1, 2}, {3, 2}, {0, 3},
	 {5, 4}, {5, 6}, {6, 7}, {4, 7},
	 {3, 4}, {0, 5}, {1, 6}, {2, 7}};

// foreach segment ith in an OBB gets the axis that's parallel to it (0 = x, 1 = y, 2 = z)
static const u8 OBBSegmentsPD[NUMOBBSEGMENTS][2] = 
	{{0, 0}, {1, 2}, {3, 0}, {0, 2},
	 {5, 2}, {5, 0}, {6, 2}, {4, 0},
	 {3, 1}, {0, 1}, {1, 1}, {2, 1}};

enum ContactPointType
{
	BOXCOLLISION
};

struct ContactPointStruct
{
	vector3 position;
	vector3 normal;
	u16 penetration;
	void* target;
	ContactPointType type;
};

struct OBBStruct
{
	int32 transformationMatrix[9]; //3x3

	vector3 position;
    vector3 size;

	/// SAT collision detection stuff
	vector3 AABBposition;
	vector3 AABBsize;
	u8 numContactPoints;
	ContactPointStruct contactPoints[NUMOBBSEGMENTS];

	OBBStruct() = default;
};

#include "LUTs.hpp"

// helper functions
/// given a size, position, normals, stores the vertices of a box in v
void getBoxVertices(vector3 size, vector3 position, vector3 u1, vector3 u2, vector3 u3, vector3* v);
// THIS MODIFIES THE OBB STRUCT PASSED, basically wraps getBoxVertices and then creates an AABB grown around the OBB passed
void getOBBVerticesAndCreateAABB(OBBStruct* obb, vector3* v);
// clipping function
bool clipSegmentOBB(int32* ss, vector3 *uu, vector3* p1, vector3* p2, vector3 vv, vector3* uu1, vector3* uu2, vector3 vv1, vector3* n1, vector3* n2, bool* b1, bool* b2, int32* k1, int32* k2);
// returns p projected in the AABB described by size. stores the face's normal in n
vector3 projectPointAABB(vector3 size, vector3 p, vector3* n);

// OBBs managment
void initOBB(OBBStruct* obb, vector3 position, vector3 size, int32 mass);

// collision
bool collideAABBs(vector3 p1, vector3 s1, vector3 p2, vector3 s2);
void collideOBBs(OBBStruct* obb1, OBBStruct* obb2);

#endif