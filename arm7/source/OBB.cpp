#include "OBB.hpp"

// https://github.com/smealum/portalDS/blob/master/arm7/source/OBB.c#L476
void getVertices(vector3 size, vector3 position, vector3 u1, vector3 u2, vector3 u3, vector3* v)
{
	int32 m2[9];
	m2[0]=mulf32(u1.x,size.x);m2[3]=mulf32(u1.y,size.x);m2[6]=mulf32(u1.z,size.x);
	m2[1]=mulf32(u2.x,size.y);m2[4]=mulf32(u2.y,size.y);m2[7]=mulf32(u2.z,size.y);
	m2[2]=mulf32(u3.x,size.z);m2[5]=mulf32(u3.y,size.z);m2[8]=mulf32(u3.z,size.z);

	v[0]=vector3(-m2[0]-m2[1]-m2[2],-m2[3]-m2[4]-m2[5],-m2[6]-m2[7]-m2[8]);
	v[1]=vector3(m2[0]-m2[1]-m2[2],m2[3]-m2[4]-m2[5],m2[6]-m2[7]-m2[8]);
	v[2]=vector3(m2[0]-m2[1]+m2[2],m2[3]-m2[4]+m2[5],m2[6]-m2[7]+m2[8]);
	v[3]=vector3(-m2[0]-m2[1]+m2[2],-m2[3]-m2[4]+m2[5],-m2[6]-m2[7]+m2[8]);

	v[4]=vector3(-v[1].x,-v[1].y,-v[1].z);
	v[5]=vector3(-v[2].x,-v[2].y,-v[2].z);
	v[6]=vector3(-v[3].x,-v[3].y,-v[3].z);
	v[7]=vector3(-v[0].x,-v[0].y,-v[0].z);
	
	v[0]=addVector(v[0],position);v[1]=addVector(v[1],position);v[2]=addVector(v[2],position);v[3]=addVector(v[3],position);
	v[4]=addVector(v[4],position);v[5]=addVector(v[5],position);v[6]=addVector(v[6],position);v[7]=addVector(v[7],position);
}

bool clipSegmentOBB(int32* ss, vector3 *uu, vector3* p1, vector3* p2, vector3 vv, vector3* uu1, vector3* uu2, vector3 vv1, vector3* n1, vector3* n2, bool* b1, bool* b2, int32* k1, int32* k2)
{
	if(!p1 || !p2 || !uu1 || !uu2 || !n1 || !n2 || !b1 || !b2 || !k1 || !k2)return false;

	if(uu1->x < -ss[0])
	{
		if(uu2->x>-ss[0])
		{
			int32 k = divv16(ABS(uu1->x + ss[0]), ABS(vv1.x));
				*uu1 = addVector(*uu1, multVector(vv1, k));
				*p1 = addVector(*p1, multVector(vv, k));
			
			*k1 = MAX(*k1, k);
			*n1 = vector3(-uu[0].x, -uu[0].y, -uu[0].z);
			*b1 = true;
		}
		else return false;
	}
	else
	{
		if(uu2->x < -ss[0])
		{
			int32 k = divv16(ABS(uu1->x + ss[0]), ABS(vv1.x));
				*uu2 = addVector(*uu1, multVector(vv1,k));
				*p2 = addVector(*p1, multVector(vv,k));
			*k2 = MIN(*k2, k);
			*n2 = vector3(-uu[0].x, -uu[0].y, -uu[0].z);

			*b2 = true;
		}
	}
				
	if(uu1->x < ss[0])
	{
		if(uu2->x > ss[0])
		{
			int32 k = divv16(ABS(uu1->x - ss[0]), ABS(vv1.x));
				*uu2 = addVector(*uu1, multVector(vv1,k));
				*p2 = addVector(*p1, multVector(vv,k));
			*k2 = MIN(*k2, k);
			*n2 = (uu[0]);
			*b2 = true;
		}
	}
	else
	{
		if(uu2->x < ss[0])
		{
			int32 k = divv16(ABS(uu1->x - ss[0]), ABS(vv1.x));
				*uu1 = addVector(*uu1, multVector(vv1,k));
				*p1 = addVector(*p1, multVector(vv,k));
			*k1 = MAX(*k1, k);
			*n1 = (uu[0]);
			*b1 = true;
		}
		else return false;
	}

				
	if(uu1->y < -ss[1])
	{
		if(uu2->y > -ss[1])
		{
			int32 k = divv16(ABS(uu1->y + ss[1]), ABS(vv1.y));
				*uu1 = addVector(*uu1, multVector(vv1,k));
				*p1 = addVector(*p1, multVector(vv,k));
			*k1 = MAX(*k1, k);
			*n1 = vector3(-uu[1].x, -uu[1].y, -uu[1].z);
			*b1 = true;
		}
		else return false;
	}
	else
	{
		if(uu2->y < -ss[1])
		{
			int32 k = divv16(ABS(uu1->y + ss[1]), ABS(vv1.y));
				*uu2 = addVector(*uu1, multVector(vv1, k));
				*p2 = addVector(*p1, multVector16(vv, k));
			*k2 = MIN(*k2, k);
			*n2 = vector3(-uu[1].x, -uu[1].y, -uu[1].z);
			*b2 = true;
		}
	}
				
	if(uu1->y < ss[1])
	{
		if(uu2->y > ss[1])
		{
			int32 k = divv16(ABS(uu1->y - ss[1]), ABS(vv1.y));
				*uu2 = addVector(*uu1, multVector(vv1, k));
				*p2 = addVector(*p1, multVector(vv, k));
			*k2 = MIN(*k2, k);
			*n2 = (uu[1]);
			*b2 = true;
		}
	}
	else
	{
		if(uu2->y < ss[1])
		{
			int32 k = divv16(ABS(uu1->y - ss[1]), ABS(vv1.y));
				*uu1 = addVector(*uu1, multVector(vv1,k));
				*p1 = addVector(*p1, multVector(vv,k));
			*k1 = MAX(*k1, k);
			*n1 = (uu[1]);
			*b1 = true;
		}
		else return false;
	}

				
	if(uu1->z < -ss[2])
	{
		if(uu2->z > -ss[2])
		{
			int32 k = divv16(ABS(uu1->z + ss[2]), ABS(vv1.z));
				*uu1 = addVector(*uu1, multVector(vv1, k));
				*p1 = addVector(*p1, multVector(vv, k));
			*k1 = MAX(*k1, k);
			*n1 = vector3(-uu[2].x, -uu[2].y, -uu[2].z);
			*b1 = true;
		}
		else return false;
	}
	else
	{
		if(uu2->z<-ss[2])
		{
			int32 k = divv16(ABS(uu1->z + ss[2]), ABS(vv1.z));
				*uu2 = addVector(*uu1, multVector(vv1, k));
				*p2 = addVector(*p1, multVector(vv, k));
			*k2 = MIN(*k2, k);
			*n2 = vector3(-uu[2].x, -uu[2].y, -uu[2].z);
			*b2 = true;
		}
	}
				
	if(uu1->z < ss[2])
	{
		if(uu2->z > ss[2])
		{
			int32 k = divv16(ABS(uu1->z - ss[2]), ABS(vv1.z));
				*uu2 = addVector(*uu1, multVector(vv1,k));
				*p2 = addVector(*p1, multVector(vv, k));
			*k2 = MIN(*k2, k);
			*n2 = (uu[2]);
			*b2 = true;
		}
	}
	else
	{
		if(uu2->z < ss[2])
		{
			int32 k = divv16(ABS(uu1->z - ss[2]), ABS(vv1.z));
				*uu1 = addVector(*uu1, multVector(vv1,k));
				*p1 = addVector(*p1, multVector(vv,k));
			*k1 = MAX(*k1, k);
			*n1 = (uu[2]);
			*b1 = true;
		}
		else return false;
	}
	return true;
}

vector3 projectPointAABB(vector3 size, vector3 p, vector3* n)
{
	if (!n)
		return vector3_zero;
	
	vector3 v = p;
	*n = vector3_zero;

	if (p.x<-size.x)		{v.x =-size.x;	n->x=-1;}
	else if (p.x>size.x) 	{v.x =size.x;	n->x=1;	}
	else if (p.y<-size.y)	{v.y =-size.y;	n->y=-1;}
	else if (p.y>size.y) 	{v.y =size.y;	n->y=1;	}
	else if (p.z<-size.z)	{v.z =-size.z;	n->z=-1;}
	else if (p.z>size.z) 	{v.z =size.z;	n->z=1;	}

	if(!n->x && !n->y && !n->z)
	{	
		int32 d1=ABS(p.x+size.x); int32 d2=ABS(p.x-size.x);
		int32 d3=ABS(p.y+size.y); int32 d4=ABS(p.y-size.y);
		int32 d5=ABS(p.z+size.z); int32 d6=ABS(p.z-size.z);
		
		int32 d=MIN(d1,d2);
		d=MIN(d,MIN(d3,d4));
		d=MIN(d,MIN(d5,d6));

		if (d==d1)		{v.x = -size.x;	n->x=-1;}
		else if(d==d2)	{v.x = size.x;	n->x=1;	}
		else if(d==d3)	{v.y = -size.y;	n->y=-1;}
		else if(d==d4)	{v.y = size.y;	n->y=1;	}
		else if(d==d5)	{v.z = -size.z;	n->z=-1;}
		else {v.z=size.z;n->z=1;}
	}

	return v;
}

void getOBBVerticesAndCreateAABB(OBBStruct* obb, vector3* v)
{
    if(!obb || !v)
        return;
    
    int32* obbm = obb->transformationMatrix;
    getVertices(
        obb->size,
        obb->position,
        vector3(obbm[0], obbm[3], obbm[6]),
        vector3(obbm[1], obbm[4], obbm[7]),
        vector3(obbm[2], obbm[5], obbm[8]),
        v
    );

    // now create the AABB and store it in the OBBStruct
    vector3 AABBposition = obb->position;
    vector3 AABBsize = obb->size;
    for (size_t i = 0; i < 8; i++)
    {
        // just basically grow and fill the OBB
        if(v[i].x<AABBposition.x) AABBposition.x=v[i].x;
		if(v[i].y<AABBposition.y) AABBposition.y=v[i].y;
		if(v[i].z<AABBposition.z) AABBposition.z=v[i].z;
		if(v[i].x>AABBsize.x) AABBsize.x=v[i].x;
		if(v[i].y>AABBsize.y) AABBsize.y=v[i].y;
		if(v[i].z>AABBsize.z) AABBsize.z=v[i].z;
    }

    obb->AABBposition = AABBposition;
    obb->size = vectorDifference(AABBsize, AABBposition);
}

bool collideAABBs(vector3 p1, vector3 s1, vector3 p2, vector3 s2)
{
    return !(p2.x > p1.x + s1.x || p2.y > p1.y + s1.y || p2.z > p1.z + s1.z || p2.x + s2.x < p1.x || p2.y + s2.y < p1.y || p2.z + s2.z <= p1.z);
}

void collideOBBs(OBBStruct* obb1, OBBStruct* obb2)
{
    if(!obb1 || !obb2)
        return;
    if(obb1 == obb2)
        return;

    // [to do] add penetration offset to the box
    if(!collideAABBs(
        obb1->position, obb1->size,
        obb2->position, obb2->size
    ))
        return;
    
    // https://www.geometrictools.com/Documentation/DynamicCollisionDetection.pdf
    // https://github.com/smealum/portalDS/blob/master/arm7/source/OBB.c#L326
    
    // some terminology : obb = oriented bounding box, aabb = axis-aligned bounding box

    // the rough idea here is:
    // - project obb1's vertices to obb2's local space (obb2 turns into an aabb)
    // - use liang-barsky clipping to determine clipping between obb1's edges and obb2's slab (determined by obb2's aabb)

    // get OBBs normals from transformation matrices
    // obb1's normals
    vector3 z1 = vector3(obb1->transformationMatrix[0], obb1->transformationMatrix[3], obb1->transformationMatrix[6]);
    vector3 z2 = vector3(obb1->transformationMatrix[1], obb1->transformationMatrix[4], obb1->transformationMatrix[7]);
    vector3 z3 = vector3(obb1->transformationMatrix[2], obb1->transformationMatrix[5], obb1->transformationMatrix[8]);
    // obb2's normals
    vector3 u1 = vector3(obb2->transformationMatrix[0], obb2->transformationMatrix[3], obb2->transformationMatrix[6]);
    vector3 u2 = vector3(obb2->transformationMatrix[1], obb2->transformationMatrix[4], obb2->transformationMatrix[7]);
    vector3 u3 = vector3(obb2->transformationMatrix[2], obb2->transformationMatrix[5], obb2->transformationMatrix[8]);

    // obb2 is our reference!
    vector3 d = vectorDifference(obb1->position, obb2->position);

    // transform obb1's vertices in obb2's local space
    vector3 obb1vt2[8]; // obb1's vertices transformed -> 2
    getBoxVertices(
        obb1->size,
        vector3(dotProduct(d, u1), dotProduct(d, u2), dotProduct(d, u3)),       // position to obb2 local space
        vector3(dotProduct(z1, u1), dotProduct(z1,u2), dotProduct(z1,u3)),      // obb x normal to obb2 space
		vector3(dotProduct(z2, u1), dotProduct(z2, u2), dotProduct(z2, u3)),    // y normal
        vector3(dotProduct(z3, u1), dotProduct(z3, u2), dotProduct(z3,u3)),     // z normal
        obb1vt2
    );

    // extract obb2 vertices as well and construct obb2's aabb
    vector3 obb2vaabb[8];
    getOBBVerticesAndCreateAABB(obb2, obb2vaabb);

    // arrays for procedural code in the for loop below ∨
    vector3 z[3] = { z1, z2, z3 };
    vector3 u[3] = { u1, u2, u3 };
    int32 obb1s[3] = { obb1->size.x, obb1->size.y, obb1->size.z };
    int32 obb2s[3] = { obb2->size.x, obb2->size.y, obb2->size.z };

    // okay finally let's get into clipping space
    for (size_t i = 0; i < NUMOBBSEGMENTS; i++)
    {
        // we have to determmine if the edge (segment, which we'll call uu) is clipping the AABB's region or not
        vector3 uu1 = obb1vt2[OBBSegments[i][0]];   // start point of the segment. fetch it from the look up table ([0])
        vector3 uu2 = obb1vt2[OBBSegments[i][1]];   // end point of the segment

        // is the segment in any axis in front of the AABB?
        const bool t = !
            ((uu1.x < -obb2s[0] && uu2.x < -obb2s[0]) || (uu1.x > obb2s[0] && uu2.x > obb2s[0])) ||
            ((uu1.y < -obb2s[1] && uu2.y < -obb2s[1]) || (uu1.y > obb2s[1] && uu2.y > obb2s[1])) ||
            ((uu1.z < -obb2s[2] && uu2.z < -obb2s[2]) || (uu1.z > obb2s[2] && uu2.z > obb2s[2]));
        
        if(t)
        {
            do
            {
                /// if this edge survives all three slab‐clips, generate one or two contact points
                const vector3 vv = z[OBBSegmentsPD[i][1]]; // world-space direction of segment i
                const vector3 vv1 = vector3(               // transform vv in obb2's local space
                    dotProduct(vv, z[0]),
                    dotProduct(vv, z[1]),
                    dotProduct(vv, z[2])
                );

                // prepare stuff for clipSegmentCall
                // (p1 and p2 will be modified)
                vector3 p1 = uu1;
                vector3 p2 = uu2;

                vector3 n1 = vector3_zero;
                vector3 n2 = vector3_zero;
                int32 k1 = 0;
                int32 k2 = obb1s[OBBSegmentsPD[i][1]] * 2;  // length
                bool b1, b2 = false;

                if(!clipSegmentOBB(obb2s, u, &p1, &p2, vv, &uu1, &uu2, vv1, &n1, &n2, &b1, &b2, &k1, &k2))
					break;

				obb1->numContactPoints = 0;	// contact points data will be left dirty

                if(b1 && b2)    // segmented entered and exited
                {
					obb1->contactPoints[obb1->numContactPoints].position = vectorDivInt(addVector(p1, p2), 2);	// average
					obb1->contactPoints[obb1->numContactPoints].type = BOXCOLLISION;

					vector3 n;
					vector3 oo = vectorDivInt(addVector(uu1, uu2), 2);
					vector3 p  = projectPointAABB(obb2->size, oo, &n);

					obb1->contactPoints[obb1->numContactPoints].normal = vector3(
						n.x * u1.x + n.y * u2.x + n.z * u3.x,
						n.x * u1.y + n.y * u2.y + n.z * u3.y,
						n.x * u1.z + n.y * u2.z + n.z * u3.z
					);
					obb1->contactPoints[obb1->numContactPoints].penetration = distance(p, oo);
					obb1->contactPoints[obb1->numContactPoints].target = obb2;

					obb1->numContactPoints++;
                }
                else
                {
                    if(b1)
                    {
						obb1->contactPoints[obb1->numContactPoints].position = p1;
						obb1->contactPoints[obb1->numContactPoints].type = BOXCOLLISION;
						obb1->contactPoints[obb1->numContactPoints].normal = n1;
						obb1->contactPoints[obb1->numContactPoints].penetration = 0;
						obb1->contactPoints[obb1->numContactPoints].target = obb2;
						obb1->numContactPoints++;
                    }
                    if(b2)
                    {
						obb1->contactPoints[obb1->numContactPoints].position = p2;
						obb1->contactPoints[obb1->numContactPoints].type = BOXCOLLISION;
						obb1->contactPoints[obb1->numContactPoints].normal = n2;
						obb1->contactPoints[obb1->numContactPoints].penetration = 0;
						obb1->contactPoints[obb1->numContactPoints].target = obb2;
						obb1->numContactPoints++;
                    }
                }
            }
            while(0);
        }
    }
    
}