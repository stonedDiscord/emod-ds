#ifndef MATH_HPP_7
#define MATH_HPP_7

#if defined(ARM7)

#define inttof32(n)		((n) << (12))
#define f32toint(n)		((n) >> (12))
#define floattof32(n)	((int)((n) * (1 << (12))))
#define f32tofloat(n)	(((float)(n)) / (float)(1<<(12)))

#endif

#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define ABS(a) ((a)>=0?(a):(-a))
#define CLAMP(x, lo, hi)  \
    (((x) < (lo)) ? (lo) \
    : ((x) > (hi)) ? (hi) \
    : (x))

#define vector3(x,y,z) ((vector3){(x),(y),(z)})
#define vector3_zero ((vector3){0, 0, 0})
#define vector2(x,y) ((vector2){(x),(y)})
#define vector2_zero ((vector2){0, 0})

#define fixedDelta (floattof32(1.0f/30.0f))

#include "LUTs.hpp"

typedef struct
{
	int32 x, y, z;
} vector3;

typedef struct
{
	int32 x, y;
} vector2;


#if defined(ARM7)

static inline int32 mulf32(int32 a, int32 b)
{
	long long result = (long long)a * (long long)b;
	return (int32)(result >> (12));
}

#endif

static inline int32 divv16(int32 a, int32 b)
{
	if (b == 0)
		return 0;

	int32 d=(((int32)a)<<12)/b;
	return (int32)d;
}

static inline int32 mulv16(int32 a, int32 b)
{
	int32 d=((int32)a)*((int32)b);
	return (int32)(d>>12);
}

static inline vector3 addVector(vector3 p1, vector3 p2)
{
	return vector3(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
}

static inline vector3 multVector(vector3 v, int32 k)
{
	return vector3(mulf32(v.x,k), mulf32(v.y,k), mulf32(v.z,k));
}

static inline vector3 multVector16(vector3 v, int32 k)
{
	return vector3(mulv16(v.x,k), mulv16(v.y,k), mulv16(v.z,k));
}

static inline vector3 multVectorInt(vector3 v, int k)
{
	return vector3((v.x*k), (v.y*k), (v.z*k));
}

static inline vector3 vectorDivInt(vector3 v, int k)
{
	return vector3((v.x/k), (v.y/k), (v.z/k));
}

static inline vector3 vectorDifference(vector3 p1, vector3 p2)
{
	return vector3(p1.x - p2.x, p1.y-p2.y, p1.z-p2.z);
}

static inline int32 dotProduct(vector3 v1, vector3 v2)
{
	return (mulf32(v1.x,v2.x) + mulf32(v1.y,v2.y) + mulf32(v1.z,v2.z));
}

static inline vector3 normalize(vector3 v)
{
	int32 d = sqrtv(mulf32(v.x,v.x) + mulf32(v.y,v.y) + mulf32(v.z,v.z));
	return vector3(divv16(v.x,d), divv16(v.y,d), divv16(v.z,d));
}

static inline int32 magnitude(vector3 v)
{
	int32 d = sqrtv(mulf32(v.x, v.x) + mulf32(v.y, v.y) + mulf32(v.z, v.z));
	return d;
}

static inline int64 sqMagnitude(vector3 v)
{
	int64 d = (int64)(v.x * v.x) + (int64)(v.y * v.y) + (int64)(v.z * v.z);
	return d;
}

static inline int32 distance(vector3 v1, vector3 v2)
{
	return magnitude(vectorDifference(v2, v1));
}

/*

static inline vector3 divideVect(vector3 v, int32 d)
{
	return vect(divv16(v.x,d),divv16(v.y,d),divv16(v.z,d));
}

static inline vector3 vectProduct(vector3 v1, vector3 v2)
{
	return vect(mulf32(v1.y,v2.z)-mulf32(v1.z,v2.y),mulf32(v1.z,v2.x)-mulf32(v1.x,v2.z),mulf32(v1.x,v2.y)-mulf32(v1.y,v2.x));
}

// static inline int cosLerp(int32 x){return floattof32(cos((x*PI)/16384));}
// static inline int sinLerp(int32 x){return floattof32(sin((x*PI)/16384));}

static inline int cosLerp(int32 x){return 1;}
static inline int sinLerp(int32 x){return 1;}

static inline void multMatrix33(int32* m1, int32* m2, int32* m) //3x3
{
	int i, j;
	// for(i=0;i<4;i++)for(j=0;j<4;j++)m[i+j*4]=m1[i+0*4]*m2[0+j*4]+m1[i+1*4]*m2[1+j*4]+m1[i+2*4]*m2[2+j*4]+m1[i+3*4]*m2[3+j*4];
	for(i=0;i<3;i++)for(j=0;j<3;j++)m[j+i*3]=mulf32(m1[0+i*3],m2[j+0*3])+mulf32(m1[1+i*3],m2[j+1*3])+mulf32(m1[2+i*3],m2[j+2*3]);
}

static inline void multMatrix332(int32* m1, int32* m2, int32* m) //3x3
{
	int i, j;
	for(i=0;i<3;i++)for(j=0;j<3;j++)m[i+j*3]=mulf32(m1[i+0*3],m2[0+j*3])+mulf32(m1[i+2*3],m2[2+j*3])+mulf32(m1[i+2*3],m2[2+j*3]);
}

static inline void addMatrix33(int32* m1, int32* m2, int32* m) //3x3
{
	int i, j;
	for(i=0;i<3;i++)for(j=0;j<3;j++)m[j+i*3]=m1[j+i*3]+m2[j+i*3];
}

static inline void transposeMatrix33(int32* m1, int32* m2) //3x3
{
	int i, j;
	for(i=0;i<3;i++)for(j=0;j<3;j++)m2[j+i*3]=m1[i+j*3];
}

static inline vector3 evalVectMatrix33(int32* m, vector3 v) //3x3
{
	return vect((mulf32(v.x,m[0])+mulf32(v.y,m[1])+mulf32(v.z,m[2])),
				(mulf32(v.x,m[3])+mulf32(v.y,m[4])+mulf32(v.z,m[5])),
				(mulf32(v.x,m[6])+mulf32(v.y,m[7])+mulf32(v.z,m[8])));
}

static inline int32 clamp(int32 v, int32 m, int32 M)
{
	if(m<M)return max(m,min(v,M));
	else return min(m,max(v,M));
}

static inline void rotateMatrixX(int32* tm, int32 x, bool r)
{
	int i;
	int32 rm[9], m[9];
	for(i=0;i<9;i++)rm[i]=0;
	rm[0]=inttof32(1);
	rm[4]=cosLerp(x);
	rm[5]=sinLerp(x);
	rm[7]=-sinLerp(x);
	rm[8]=cosLerp(x);
	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

static inline void rotateMatrixY(int32* tm, int32 x, bool r)
{
	int i;
	int32 rm[9], m[9];
	for(i=0;i<9;i++)rm[i]=0;
	rm[0]=cosLerp(x);
	rm[2]=sinLerp(x);
	rm[4]=inttof32(1);
	rm[6]=-sinLerp(x);
	rm[8]=cosLerp(x);
	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

static inline void rotateMatrixZ(int32* tm, int32 x, bool r)
{
	int i;
	int32 rm[9], m[9];
	for(i=0;i<9;i++)rm[i]=0;
	rm[0]=cosLerp(x);
	rm[1]=sinLerp(x);
	rm[3]=-sinLerp(x);
	rm[4]=cosLerp(x);
	rm[8]=inttof32(1);
	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

static inline void rotateMatrixAxis(int32* tm, int32 x, vector3 a, bool r)
{
	int32 rm[9], m[9];

	int32 cosval=cosLerp(x);
	int32 sinval=sinLerp(x);
	int32 onemcosval=inttof32(1)-cosval;

	rm[0]=cosval+mulf32(mulf32(a.x,a.x),onemcosval);
	rm[1]=mulf32(mulf32(a.x,a.y),onemcosval)-mulf32(a.z,sinval);
	rm[2]=mulf32(mulf32(a.x,a.z),onemcosval)+mulf32(a.y,sinval);

	rm[3]=mulf32(mulf32(a.x,a.y),onemcosval)+mulf32(a.z,sinval);
	rm[4]=cosval+mulf32(mulf32(a.y,a.y),onemcosval);
	rm[5]=mulf32(mulf32(a.y,a.z),onemcosval)-mulf32(a.x,sinval);

	rm[6]=mulf32(mulf32(a.x,a.z),onemcosval)-mulf32(a.y,sinval);
	rm[7]=mulf32(mulf32(a.y,a.z),onemcosval)+mulf32(a.x,sinval);
	rm[8]=cosval+mulf32(mulf32(a.z,a.z),onemcosval);

	if(r)multMatrix33(rm,tm,m);
	else multMatrix33(tm,rm,m);
	memcpy(tm,m,9*sizeof(int32));
}

static inline void fixMatrix(int32* m) //3x3
{
	if(!m)return;
	vector3 x=vect(m[0],m[3],m[6]);
	vector3 y=vect(m[1],m[4],m[7]);
	vector3 z=vect(m[2],m[5],m[8]);
	
	projectVectorPlane(&x,y);
	projectVectorPlane(&z,y);
	projectVectorPlane(&z,x);
	
	x=normalize(x);
	y=normalize(y);
	z=normalize(z);
	
	m[0]=x.x;m[3]=x.y;m[6]=x.z;
	m[1]=y.x;m[4]=y.y;m[7]=y.z;
	m[2]=z.x;m[5]=z.y;m[8]=z.z;
}

*/

static inline void projectVectorPlane(vector3* v, vector3 n)
{
	if (!v)
		return;
	int32 r = dotProduct(*v, n);
	*v = vectorDifference(*v, multVector(n,r));
}

static inline vector3 lerpVector3(vector3 v1, vector3 v2, int32 t)
{
	return addVector(v1, multVector(vectorDifference(v2, v1), t));
}

static inline int32 lerpInt(int32 a, int32 b, int32 t)
{
	return a + mulf32(b - a, t);
}

#endif