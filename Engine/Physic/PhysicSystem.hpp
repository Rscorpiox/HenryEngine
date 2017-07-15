#pragma once

#ifndef PHYSICSYSTEM_HPP 
#define PHYSICSYSTEM_HPP

#include "Engine\Physic\CollisionObject.hpp"
#include "Engine\Physic\PhysicDebugDrawer.hpp"


namespace Henry
{ 

typedef btCollisionWorld::ClosestRayResultCallback ClosestRaycastResult;

class PhysicSystem
{
public:
	PhysicSystem();
	~PhysicSystem();
	void SetGravity(float x, float y, float z);
	btVector3 GetGravity();
	void Update(double deltaSeconds, int steps = 10);
	void Render();
	int AddSphere(Vec3f pos, Vec3f orient, float radius = 1.0f, float mass = 1.0f, int simulateState = ACTIVE_TAG);
	int AddPlane(Vec3f pos, Vec3f orient, Vec3f normal, float planeConstant = 1.0f, int simulateState = ACTIVE_TAG);
	int AddBox(Vec3f pos, Vec3f orient, Vec3f halfExtendSize, int simulateState = ACTIVE_TAG);
	int AddConstraint(btTypedConstraint* constraint, bool ignoreCollisionBetweetLinkedBodies = false);
	btCollisionObject* GetCollisionObjectById(int index);
	ClosestRaycastResult ClosestRaycast(Vec3f start, Vec3f end);

public:
	bool m_debugDraw;
	bool m_drawWired;

private:
	void Initialize();

private:
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;
	std::vector<btTypedConstraint*> m_constraints;
	std::vector<Vec3f> m_constrainOffsetA;
	std::vector<Vec3f> m_constrainOffsetB;
	PhysicDebugDrawer m_debugDrawer;
};

}

#endif