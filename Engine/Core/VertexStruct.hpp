#pragma once

#ifndef VertexStruct 
#define	VertexStruct

#include "Engine\Math\Vec3.hpp"
#include "Engine\Math\Vec2.hpp"

namespace Henry
{

#define MAX_BONE_SUPPORT 128
#define MAX_BONE_ATTACH_SUPPORT 8

struct RGBA
{
	RGBA(){ r = 255; g = 255; b = 255; a = 255; };
	RGBA(unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha) : r(red),g(green),b(blue),a(alpha){};
	RGBA(float red,float green,float blue,float alpha) : r( (unsigned char)(red * 255) ),b(  (unsigned char)(blue * 255) ),g(  (unsigned char)(green * 255) ),a(  (unsigned char)(alpha * 255) ) {}
	void operator*=(float rhs){ r =  (unsigned char)(r * rhs); g =  (unsigned char)( g * rhs); b =  (unsigned char)( b * rhs); a =  (unsigned char)(a * rhs); };
	RGBA operator*(float rhs) { return RGBA( (unsigned char)(r*rhs), (unsigned char)(g*rhs), (unsigned char)(b*rhs), (unsigned char)(a*rhs) ); }
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	//float* GetFloatFromRGBA() {} ;
};

struct Normal
{
	Normal() : normal(Vec3f(0,0,1)) {};
	Vec3f normal;
	Vec3f tangent;
	Vec3f biTangent;
};

struct Vertex_PosColor
{
	Vertex_PosColor(){};
	Vertex_PosColor(Vec3f pos,RGBA rgb_color) : position(pos),color(rgb_color) {};
	Vec3f position;
	RGBA color;
};

struct Vertex_PCT
{
	Vec3f position;
	RGBA color;
	Vec2f texCoords;
};

struct Vertex_PCTN
{
	Vec3f position;
	RGBA color;
	Vec2f texCoords;
	Normal normals;
};


struct BoneIdWeight
{
	BoneIdWeight()
	{
		for (int index = 0; index < MAX_BONE_ATTACH_SUPPORT; ++index)
		{
			ids[index] = 0;
			weights[index] = 0.0f;
		}
	}
	
	int ids[MAX_BONE_ATTACH_SUPPORT];
	float weights[MAX_BONE_ATTACH_SUPPORT];
};


struct Vertex_PCTNB
{
	Vec3f position;
	RGBA color;
	Vec2f texCoords;
	Normal normals;
	BoneIdWeight boneIdWeights;
};


};

#endif