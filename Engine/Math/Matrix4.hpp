#pragma once

#ifndef MATRIX4X4_HPP 
#define MATRIX4X4_HPP

#include "Vec3.hpp"

namespace Henry
{

class Matrix4
{
public:
	Matrix4(void);
	Matrix4(float* matrix);
	Matrix4(Vec3f upVector, Vec3f rightVector, Vec3f forwardVector);
	~Matrix4(void);
	void ApplyMatrix(const Matrix4* matrix);
	void ApplyMatrix(const float* matrix);
	void ApplyTranslation(float x,float y,float z);
	void ApplyRotateDegrees(float degrees, float x, float y, float z);
	void ApplyRotateRadians(float radians, float x, float y, float z);
	void ApplyScale(float x,float y,float z);
	void ApplyVector(Vec3f& vector);
	void LoadIdentity();
	void Transpose();
	bool Inverse();
	Vec3f GetTranslation();
	Vec3f GetEulerDegrees();
	Vec3f GetRotationRadians();
	Vec3f GetScale();
	//Matrix4 GetTransformMatrix(const float* matrix);

public:
	float m_matrix[16];
};

};

#endif