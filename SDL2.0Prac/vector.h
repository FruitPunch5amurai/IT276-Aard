
#ifndef __VECTOR_H_
#define __VECTOR_H_


typedef struct
{
	float x , y;
}Vec2d;


typedef struct
{
	union {float x; float y;};
	union {float y; float g;};
	union {float z; float b;};
}Vec3d;


typedef struct
{
	union {float x; float y;};
	union {float y; float g;};
	union {float z; float b;};
	union {float w; float a;};
}Vec4d;

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


#endif
