#ifndef __VECTORS_H_
#define __VECTORS_H_


typedef struct
{
	float x , y;
}Vec2D;


typedef struct
{
	union {float x; float y;};
	union {float y; float g;};
	union {float z; float b;};
}Vec3D;


typedef struct
{
	union {float x; float y;};
	union {float y; float g;};
	union {float z; float b;};
	union {float w; float a;};
}Vec4D;
/*
typedef struct
{


}circle;

typedef struct
{


}triangle;
typedef struct
{


)rect;
enum Shapes
{
	Shape_Rect,
	Shape_Circle,
	Shape_Triangle
};

typedef struct
{
	int selection;
	union {
		circle c;
		rect r;
		triangle t;
}shape;
*/

/**
*@brief Methods to create Vecs
*/

Vec2D CreateVec2D(int x , int y);
Vec3D CreateVec3D(int x , int y, int z);
Vec4D CreateVec4D(int x , int y,int z, int w);

#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)

void Vec3CrossProduct(Vec3D * out, Vec3D vec1,Vec3D vec2);

/**
 * @brief Macro to get the dot product from a vector.
 *
 * @param a MglVect3D component of the dot product
 * @param b MglVect3D component of the dot product
 *
 * @return the calculated dot product
 */
#define Vec4DDotProduct(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w)
#define Vec3DDotProduct(a,b)      (a.x*b.x+a.y*b.y+a.z*b.z)
#define Vec2DDotProduct(a,b)      (a.x*b.x+a.y*b.y)
/**
 * @brief checks if vectors are exactly matches of each other
 *
 * @param a one vector to check
 * @param b other vector to check
 */
#define Vec2DEqual(a,b) ((a.x == b.x)&&(a.y == b.y))
#define Vec3DEqual(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z))
#define Vec4DEqual(a,b) ((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z)&&(a.w == b.w))

/**
 * @brief Macro to subtract two vectors
 * varient ending in p takes a pointer to MglVect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result MglVect3D output
 * @param a MglVect3D input
 * @param b MglVect3D input
 */
#define Vec2DSub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y)
#define Vec3DSub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z)
#define Vec4DSub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y,dst.z=a.z-b.z,dst.w=a.w-b.w)

/**
 * @brief Macro to add two vectors
 * varient ending in p takes a pointer to MglVect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result MglVect3D output
 * @param a MglVect3D input
 * @param b MglVect3D input
 */

#define Vec2DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)
#define Vec3DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z)
#define Vec4DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y,dst.z = a.z+b.z,dst.w = a.w+b.w)


/**
 * @brief Macro to scale a vector by a scalar value
 * varient ending in p takes a pointer to MglVect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result MglVect3D output
 * @param src MglVect3D input
 * @Param factpr the scalar value to scale the vector by.
 */
#define Vec2DScale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor)
#define Vec3DScale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor,\
                                         dst.z = src.z *factor)
#define Vec4DScale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor,\
                                         dst.z = src.z *factor,\
                                         dst.w = src.w *factor)
/**
 * @brief Macro that sets vector to zero.
 * @param a MglVect[2D|3D|4D] input
 */

#define Vec2DClear(a)       (a.x=a.y=0)
#define Vec3DClear(a)       (a.x=a.y=a.z=0)
#define Vec4DClear(a)       (a.x=a.y=a.z=a.w=0)

/**
 * @brief Macro to set the components of the vector
 *
 * @param v MglVect3D output
 * @param a MglFloat x component
 * @param b MglFloat y component
 * @param c MglFloat z component (only in 3D & 4D version)
 * @param d MglFloat w component (only in 4D version)
 */
#define Vec2DSet(v, a, b)  (v.x=(a), v.y=(b))
#define Vec3DSet(v, a, b, c)  (v.x=(a), v.y=(b), v.z=(c))
#define Vec4DSet(v, a, b, c,d)  (v.x=(a), v.y=(b), v.z=(c), v.w=(d))

/**
 * @brief Macro to get the negative of a vector
 *
 * @param src MglVect[2D|3D|4D] input
 * @param dst MglVect[2D|3D|4D] negated output
 */
#define Vec2DNegate(dst,src)      (dst.x = -src.x,dst.y = -src.y)
#define Vec3DNegate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z)
#define Vec4DNegate(dst,src)      (dst.x = -src.x,dst.y = -src.y,dst.z = -src.z,dst.w = -src.w)

/**
 * @brief normalizes the vector passed.  does nothing for a zero length vector.
 *
 * @param v pointer to the vector to be normalized.
 */
float Vec2DMagnitude(Vec2D V);
float Vec3DMagnitude(Vec3D V);
float Vec4DMagnitude(Vec4D V);

void Vec2DNormalize (Vec2D *V);
void Vec3DNormalize (Vec3D *V);
void Vec4DNormalize (Vec4D *V);

/**
 * @brief returns the magnitude squared, which is faster than getting the magnitude
 * which would involve taking the square root of a floating point number.
 * @param V the vector to get the magnitude for
 * @return the square of the magnitude of V
 */
float Vec2DMagnitudeSquared(Vec2D V);
float Vec3DMagnitudeSquared(Vec3D V);
float Vec4DMagnitudeSquared(Vec4D V);

/**
 * @brief scales the vector to the specified length without changing direction
 * No op is magnitude is 0 or V is NULL
 * @param V a pointer to the vector to scale
 * @param magnitude the new length for the vector
 */
void Vec2DSetMagnitude(Vec2D * V,float magnitude);
void Vec3DSetMagnitude(Vec3D * V,float magnitude);
void Vec4DSetMagnitude(Vec4D * V,float magnitude);
/**
 * @brief checks if the distance between the two points provided is less than size.
 * @param p1 one point for the distance check
 * @param p2 another point for the distance check
 * @param size the value to check against
 * @return MglTrue if the distance between P1 and P2 is less than size, false otherwise
 */
bool DistanceBetweenLessThan2D(Vec3D p1,Vec3D p2,float size);
bool DistanceBetweenLessThan3D(Vec3D p1,Vec3D p2,float size);
bool DistanceBetweenLessThan4D(Vec3D p1,Vec3D p2,float size);

/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between 0 and 1.0
 */
#define random()  ((rand ()%1000)/(float)1000.0)

/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between -1.0 and 1.0
 */
#define crandom() (((float)((rand()%1000)/(float)1000.0) * 2.0) - 1.0)


//GetLength
//GetLength
//GetAngle
//AngleToVector
//Rotate

#endif
