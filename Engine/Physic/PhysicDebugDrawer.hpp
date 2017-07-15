#pragma once

#include "Engine\Physic\Bullet3\LinearMath\btIDebugDraw.h"
#include "Engine\Renderer\OpenGLRenderer.hpp"

#ifndef PHYSICDEBUGDRAWER 
#define PHYSICDEBUGDRAWER

namespace Henry
{ 

class PhysicDebugDrawer : public btIDebugDraw
{
public:
	PhysicDebugDrawer();
	~PhysicDebugDrawer();
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	virtual void reportErrorWarning(const char* warningString);
	virtual void draw3dText(const btVector3& location, const char* textString);
	virtual void setDebugMode(int debugMode);
	virtual int getDebugMode() const;

private:
	int m_debugMode;
	float m_lineWidth;
};

}

#endif