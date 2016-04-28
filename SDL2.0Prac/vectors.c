#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <assert.h>
#include <math.h>
#include "vectors.h"




/**
*@brief Creates a 2D Vector
*/
Vec2D CreateVec2D(float x, float y)
{
  Vec2D vec;
  Vec2DSet(vec,x, y);
  return vec;
}
/**
*@brief Creates a 3D Vector
*/

Vec3D CreateVec3D(int x, int y, int z)
{
  Vec3D vec;
  Vec3DSet(vec,x, y, z);
  return vec;
}
/**
*@brief Creates a 4D Vector
*/
Vec4D CreateVec4D(int x, int y,int z,int w)
{
  Vec4D vec;
  Vec4DSet(vec,x, y, z, w);
  return vec;
}
/**
*@brief Returns the magnitude of specified 2D Vector
*/
float Vec2DMagnitude (Vec2D V)
{
  return sqrt (V.x * V.x + V.y * V.y);
}
/**
*@brief Returns the magnitude of specified 3D Vector
*/
float Vec3DMagnitude (Vec3D V)
{
  return sqrt (V.x * V.x + V.y * V.y + V.z * V.z);
}
/**
*@brief Returns the magnitude of specified 4D Vector
*/
float Vec4DMagnitude (Vec4D V)
{
  return sqrt (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}

/**
*@brief Returns the magnitude squared of specified 2D Vector
*/
float Vec2DMagnitude_squared(Vec2D V)
{
  return (V.x * V.x + V.y * V.y);
}
/**
*@brief Returns the magnitude squared of specified 3D Vector
*/
float Vec3DMagnitude_squared(Vec3D V)
{
  return (V.x * V.x + V.y * V.y + V.z * V.z);
}
/**
*@brief Returns the magnitude squared of specified 4D Vector
*/
float Vec4DMagnitude_squared(Vec4D V)
{
  return (V.x * V.x + V.y * V.y + V.z * V.z + V.w * V.w);
}


/**
*@brief Sets the magnitude squared of specified 2D Vector
*/
void Vec2DSetMagnitude(Vec2D * V,float magnitude)
{
  if (!V)return;
  Vec2DNormalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
}

/**
*@brief Sets the magnitude squared of specified 3D Vector
*/
void Vec3DSetMagnitude(Vec3D * V,float magnitude)
{
  if (!V)return;
  Vec3DNormalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
  V->z *= magnitude;
}
/**
*@brief Sets the magnitude squared of specified 4D Vector
*/
void Vec4DSetMagnitude(Vec4D * V,float magnitude)
{
  if (!V)return;
 Vec4DNormalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
  V->z *= magnitude;
  V->w *= magnitude;
}
/**
*@brief Normalizes the specified 2D Vector
*/
void Vec2DNormalize (Vec2D *V)
{
  float M;
  if (!V)return;
  M = Vec2DMagnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
}
/**
*@brief Normalizes the specified 3D Vector
*/
void Vec3DNormalize (Vec3D *V)
{
  float M;
  if (!V)return;
  M= Vec3DMagnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
  V->z *= M;
}
/**
*@brief Normalizes the specified 4D Vector
*/
void Vec4DNormalize (Vec4D *V)
{
  float M;
  if (!V)return;
  M = Vec4DMagnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  M = 1/M;
  V->x *= M;
  V->y *= M;
  V->z *= M;
  V->w *= M;
}
/**
*@brief Allocates memory for a new 2D Vector and returns it;
*/
Vec2D *Vec2D_New()
{
  Vec2D *vec = NULL;
  vec = (Vec2D *)malloc(sizeof(Vec2D));
  if (vec == NULL)
  {
    return NULL;
  }
  Vec2DClear((*vec));
  return vec;
}
void Vec3DCrossProduct(Vec3D *out, Vec3D v1, Vec3D v2)
{
  out->x = v1.y*v2.z - v1.z*v2.y;
  out->y = v1.z*v2.x - v1.x*v2.z;
  out->z = v1.x*v2.y - v1.y*v2.x;
}

bool DistanceBetweenLessThan2D(Vec2D p1,Vec2D p2,float size)
{
	float i; 
	i = sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2));
	if(sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2)) < size)
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool DistanceBetweenGreaterThan2D(Vec2D p1,Vec2D p2,float size)
{
		float i; 
	i = sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2));
	if(sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2)) > size)
	{
		return true;
	}
	else
	{
		return false;
	}

}
void Vec2DReflect(Vec2D *out, Vec2D normal,Vec2D in)
{
  float f;
  f = Vec2DDotProduct(in,normal);
  out->x = in.x - (2 * normal.x * f);
  out->y = in.y - (2 * normal.y * f);
}