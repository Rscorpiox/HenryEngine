#pragma once

#ifndef HENRYFUNCTIONS_HPP 
#define HENRYFUNCTIONS_HPP

#include "Engine\Math\Matrix4.hpp"

#include <stdlib.h>
#include <vector>

namespace Henry
{
inline float degree2radians(float degrees){ return degrees*0.0174f;  /* PI / 180 */ }
inline float radians2degrees(float radians){ return radians*57.2957795f;}
inline float getRandomPercent(){ const int MOD_MASK_2047 = 2047; float result = (float)((rand() & MOD_MASK_2047) * 0.0009765625f); return result > 1.0f ? result - 1.0f : result; /* 1 / 1024 */ };
inline float random(float min,float max){ max = max < min ? min : max; return min + (max - min) * getRandomPercent(); };
inline int random(int min, int max) { max = max < min ? min : max; float percent = 1.0f; while (percent == 1.0f){ percent = getRandomPercent(); } return (int)(min + (max - min + 1) * percent); };
inline float getDegree(Vec2f direction){ return atan2(direction.y, direction.x) * 57.295779f; };
inline float clamp(float value, float min, float max){ return value > max ? max : (value < min ? min : value); };
inline Vec3f getOrientation(Vec3f v)
{
	float r = v.length();
	float p = acos(v.z / r);
	float t = atan2(v.y, v.x);
	
	r = radians2degrees(r);
	p = radians2degrees(p);
	t = radians2degrees(t);
	
	Vec3f orient;
 	orient.x = r * sin(p) * cos(t);
 	orient.y = r * sin(p) * sin(t);
 	orient.z = r * cos(p);
 
 	orient.x = radians2degrees(orient.x);
 	orient.y = radians2degrees(orient.y);
 	orient.z = radians2degrees(orient.z);

	return orient;
}
inline Vec3f projectOntoPlane(Vec3f vector, Vec3f planeNormal, Vec3f point = Vec3f())
{
	Vec3f proj;
	proj = vector - planeNormal * (vector - point).dotProductWith(planeNormal);
	return proj;
}
inline Matrix4 QuatToMatrix(float qx, float qy, float qz, float qw)
{
	Matrix4 mat;
	mat.m_matrix[0] = 1 - 2 * qy*qy - 2 * qz*qz; mat.m_matrix[4] = 2 * qx*qy - 2 * qz*qw;	    mat.m_matrix[8] = 2 * qx*qz + 2 * qy*qw;
	mat.m_matrix[1] = 2 * qx*qy + 2 * qz*qw;	 mat.m_matrix[5] = 1 - 2 * qx*qx - 2 * qz*qz;	mat.m_matrix[9] = 2 * qy*qz - 2 * qx*qw;
	mat.m_matrix[2] = 2 * qx*qz - 2 * qy*qw;	 mat.m_matrix[6] = 2 * qy*qz + 2 * qx*qw;		mat.m_matrix[10] = 1 - 2 * qx*qx - 2 * qy*qy;
	return mat;
};
void circleTable(double **sint,double **cost,const int n);

bool ComputeSurfaceTangentsAtVertex(
	Vec3f& surfaceTangentAtVertex_out,
	Vec3f& surfaceBitangentAtVertex_out,
	const Vec3f& normalAtThisVertex,
	const Vec3f& positionOfThisVertex,
	const Vec2f& texCoordsOfThisVertex,
	const Vec3f& positionOfPreviousAdjacentVertex,
	const Vec2f& texCoordsOfPreviousAdjacentVertex,
	const Vec3f& positionOfNextAdjacentVertex,
	const Vec2f& texCoordsOfNextAdjacentVertex );

inline Vec3f ComputeSurfaceNormalAtTriangle(const Vec3f& vertexA , const Vec3f& vertexB , const Vec3f& vertexC)
{
	Vec3f vertexAToB = vertexB-vertexA;
	Vec3f vertexBToC = vertexC-vertexB;
	return vertexAToB.crossProductWith(vertexBToC);
}


void DebuggerPrintf( const char* messageFormat, ... );


template <class T>
void deleteVectorOfPointer(std::vector<T*>& pointerVector)
{
	std::vector<T*>::iterator iter = pointerVector.begin();
	while(iter != pointerVector.end())
	{
		T* temp = *iter;
		iter = pointerVector.erase(iter);
		if(temp)
		{
			delete temp;
			temp = nullptr;
		}
	}
}


void RotateAroundPoint2D(Vec2f& pointToRotate, Vec2f rotateAround, float const radians);


inline float Vector2Radians2D(Vec2f const vector2) { return atan2(vector2.y, vector2.x); };


};

#endif